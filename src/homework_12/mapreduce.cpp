#include "mapreduce.hpp"
#include "fmt/base.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

MapReduce::MapReduce(uint32_t m, uint32_t r, const std::string &file_path)
    : map_(m), reduce_(r), file_path_(file_path),
      dir_output_(std::filesystem::path(file_path).parent_path() / "output") {}

const std::string &MapReduce::GetError() const { return error_; }

std::size_t MapReduce::GetMinPrefix() const { return min_prefix_; }

bool MapReduce::Map() {
  if (!Split()) {
    return false;
  }

  active_threads_ = static_cast<uint32_t>(blocks_.size());
  for (std::string &block : blocks_) {
    std::thread(&MapReduce::TickMap, this, std::ref(block)).detach();
  }

  while (active_threads_ > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return true;
}

void MapReduce::Shuffle() {
  if (data_.size() > 1) {
    auto &first_map = data_.front();

    for (std::size_t i = 1; i < data_.size(); ++i) {
      auto &current_map = data_[i];
      first_map.insert(current_map.begin(), current_map.end());
      current_map.clear();
    }

    data_.resize(1);
  }
}

bool MapReduce::Reduce() {
  if (!PrepareOutputDir()) {
    return false;
  }

  auto &first_map = data_.front();

  std::vector<std::string> v;
  v.resize(first_map.size());

  std::size_t i{};
  for (const auto &a : first_map) {
    v[i++] = a.first;
  }

  uint32_t v_size = v.size();
  uint32_t block_size = v_size / reduce_;
  uint32_t offset = 0;
  active_threads_ = reduce_ + (v_size % reduce_ > 0 ? 1 : 0);
  int file_index = 0;

  while (v_size > 0) {
    if (v_size < block_size) {
      block_size = v_size;
    }

    std::thread(&MapReduce::TickReduce, this,
                std::vector<std::string>(v.begin() + offset,
                                         v.begin() + block_size + offset),
                ++file_index)
        .detach();

    v_size -= block_size;
    offset += block_size;
  }

  while (active_threads_ > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return true;
}

bool MapReduce::Split() {
  std::error_code e;
  auto file_size = std::filesystem::file_size(file_path_, e);
  if (e) {
    error_ = e.message();
    return false;
  }

  fmt::print("Start reading file {}\n", file_path_);

  std::ifstream file(file_path_);
  if (!file.is_open()) {
    error_ = strerror(errno);
    return false;
  }

  std::size_t block_size{static_cast<std::size_t>(file_size) / map_};
  std::size_t pos{};
  auto time_point{std::chrono::steady_clock::now()};

  std::vector<char> vec(block_size);
  while (file.read(vec.data(), block_size)) {
    fmt::print("Reading {} block...", blocks_.size() + 1);

    ProcessData(vec);

    pos += vec.size();
    file.seekg(pos);
  }

  ProcessData(vec);

  fmt::print("Read OK by {} msec\n",
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now() - time_point)
                 .count());
  return true;
}

void MapReduce::TickMap(std::string &s) {
  std::ostringstream ostream;
  ostream << std::this_thread::get_id();
  auto thread_id{ostream.str()};
  fmt::print("Start thread {}\n", thread_id);

  std::size_t reserve_size{0};
  for (auto i : s) {
    if (i == '\n') {
      ++reserve_size;
    }
  }

  std::istringstream stream(s);
  std::vector<std::string> v;
  v.reserve(reserve_size);

  auto time_point = std::chrono::steady_clock::now();

  std::string line;
  while (std::getline(stream, line)) {
    auto line_size = line.size();
    if (line_size == 0) {
      continue;
    }

    if (min_prefix_ == 0 || line_size < min_prefix_) {
      min_prefix_ = line_size;
    }

    v.emplace_back(std::move(line));
  }

  s.clear();
  s.shrink_to_fit();

  auto strings = GetStringByMinSize(v);

  {
    const auto lock = std::lock_guard{m_};
    data_.emplace_back(strings);
    --active_threads_;
  }

  fmt::print("Thread {} finished thread by {} msec\n", thread_id.c_str(),
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now() - time_point)
                 .count());
}

void MapReduce::TickReduce(const std::vector<std::string> &v, int file_index) {
  std::string file_name = "file" + std::to_string(file_index) + ".txt";

  std::ofstream file_output(dir_output_ / file_name);
  for (const auto &str : v) {
    file_output << str << '\n';
  }

  {
    const auto lock = std::lock_guard{m_};
    --active_threads_;
  }
}

void MapReduce::ProcessData(std::vector<char> &vec) {
  std::size_t erase_pos = 0;

  auto it_reverse = std::find(vec.rbegin(), vec.rend(), '\n');
  if (it_reverse == vec.rend()) {
    auto it = std::find(vec.begin(), vec.end(), '\0');
    erase_pos = std::distance(vec.begin(), it);
  } else {
    erase_pos = std::distance(vec.begin(), it_reverse.base());
  }
  if (erase_pos > 0) {
    vec.erase(vec.begin() + erase_pos, vec.end());
    blocks_.emplace_back(std::move(std::string(vec.begin(), vec.end())));

    std::fill(vec.begin(), vec.end(), '\0');
  }
}

std::unordered_map<std::string, uint32_t>
MapReduce::GetStringByMinSize(std::vector<std::string> &v) const {
  std::unordered_map<std::string, uint32_t> m;

  for (const std::string &str : v) {
    std::string part_of_string = str.substr(0, min_prefix_);

    auto it = m.find(part_of_string);
    if (it != m.end()) {
      ++m[part_of_string];
    } else {
      m[part_of_string] = 1;
    }
  }

  return m;
}

bool MapReduce::PrepareOutputDir() {
  std::error_code e;

  bool dir_exists = std::filesystem::exists(dir_output_, e);
  if (e) {
    error_ = e.message();
    return false;
  }

  if (dir_exists) {
    (void)std::filesystem::remove_all(dir_output_, e);
    if (e) {
      error_ = e.message();
      return false;
    }
  }

  (void)std::filesystem::create_directory(dir_output_, e);
  if (e) {
    error_ = e.message();
    return false;
  }

  return true;
}
