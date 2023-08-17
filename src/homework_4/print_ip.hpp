#ifndef SRC_HOMEWORK_4_PRINT_IP_HPP
#define SRC_HOMEWORK_4_PRINT_IP_HPP

#include <iostream>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

namespace homework_4 {

/**
 * @brief
 * @tparam Int целочисленный тип, представленный в виде ip
 * @param ip ip адрес для вывода
 */
template <typename T>
typename std::enable_if_t<std::is_integral<T>::value> print_ip(const T &ip) {
  for (std::size_t sz = sizeof(T); sz != 0; --sz) {
    std::cout << ((ip >> (8 * (sz - 1))) & 0xff) << (sz != 1 ? "." : "");
  }
  std::cout << std::endl;
}

template <typename T> struct IsString : std::false_type {};

template <> struct IsString<std::string> : std::true_type {};

/**
 * @brief Печатает ip с типом std::string
 * @tparam T тип std::string
 * @param ip ip
 */
template <typename T>
typename std::enable_if<IsString<T>::value>::type print_ip(const T &ip) {
  std::cout << ip << std::endl;
}

template <typename T> struct IsVectorList : std::false_type {};

template <typename... Args>
struct IsVectorList<std::list<Args...>> : std::true_type {};

template <typename... Args>
struct IsVectorList<std::vector<Args...>> : std::true_type {};

/**
 * @brief Печатает ip с типом std::vector или std::list
 * @tparam T std::vector || std::list
 * @param ip
 * @return std::enable_if_t<IsVectorList<T>::value>
 */
template <typename T>
std::enable_if_t<IsVectorList<T>::value> print_ip(const T &ip) {
  for (const auto &ip_chart : ip) {
    std::cout << ip_chart;
    std::cout << (&ip_chart != &ip.back() ? "." : "");
  }
  std::cout << std::endl;
}

template <typename T, typename... Args> struct AllSameArgs;
template <typename T> struct AllSameArgs<T> : public std::true_type {};
template <typename T, typename U, typename... Args>
struct AllSameArgs<T, U, Args...> : public std::false_type {};
template <typename T, typename... Args>
struct AllSameArgs<T, T, Args...> : public AllSameArgs<T, Args...> {};

template <typename T, std::size_t ArgPos, size_t LastDotPos>
struct FormatTuple {
  static void print_tuple_arg(const T &value) {
    std::cout << std::get<ArgPos>(value) << ".";
    FormatTuple<T, ArgPos + 1, LastDotPos>::print_tuple_arg(value);
  }
};
template <typename Type, std::size_t ArgPos>
struct FormatTuple<Type, ArgPos, ArgPos> {
  static void print_tuple_arg(const Type &value) {
    std::cout << std::get<ArgPos>(value);
  }
};

/**
 * @brief Вывод ip адреса, представленный как std::tuple, где все аргументы
 * имеют одинаковый тип
 * @tparam Args std::tuple
 * @param ip
 */
template <typename... Args>
typename std::enable_if<AllSameArgs<Args...>::value>::type
print_ip(const std::tuple<Args...> &ip) {
  constexpr int kNumArgsTuple = std::tuple_size<std::tuple<Args...>>::value;
  FormatTuple<std::tuple<Args...>, 0, kNumArgsTuple - 1>::print_tuple_arg(ip);
}

} // namespace homework_4

#endif /* SRC_HOMEWORK_4_PRINT_IP_HPP */
