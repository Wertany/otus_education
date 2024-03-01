#include <filesystem>
#include <mutex>
#include <unordered_map>
#include <vector>

class MapReduce {
public:
  MapReduce(uint32_t m, uint32_t r, const std::string &file_path);

  [[nodiscard]] const std::string &GetError() const;
  [[nodiscard]] std::size_t GetMinPrefix() const;

  bool Map();
  void Shuffle();
  bool Reduce();

private:
  std::string error_;
  uint32_t map_;
  uint32_t reduce_;
  std::string file_path_;
  std::filesystem::path dir_output_;
  std::vector<std::string> blocks_;
  uint32_t active_threads_{};
  std::mutex m_;
  std::vector<std::unordered_map<std::string, uint32_t>> data_;
  std::size_t min_prefix_{};

  bool Split();
  void TickMap(std::string &s);
  void TickReduce(const std::vector<std::string> &v, int file_index);
  void ProcessData(std::vector<char> &vec);
  std::unordered_map<std::string, uint32_t>
  GetStringByMinSize(std::vector<std::string> &v) const;
  bool PrepareOutputDir();
};
//-----------------------------------------------------------------------------
