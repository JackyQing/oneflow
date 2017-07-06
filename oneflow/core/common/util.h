#ifndef ONEFLOW_CORE_COMMON_UTIL_H_
#define ONEFLOW_CORE_COMMON_UTIL_H_

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace oneflow {

#define OF_DISALLOW_COPY(ClassName)     \
  ClassName(const ClassName&) = delete; \
  ClassName& operator=(const ClassName&) = delete;

#define OF_DISALLOW_MOVE(ClassName) \
  ClassName(ClassName&&) = delete;  \
  ClassName& operator=(ClassName&&) = delete;

#define OF_DISALLOW_COPY_AND_MOVE(ClassName) \
  OF_DISALLOW_COPY(ClassName)                \
  OF_DISALLOW_MOVE(ClassName)

#define UNEXPECTED_RUN() LOG(FATAL) << "Unexpected Run";

#define TODO() LOG(FATAL) << "TODO";

#define OF_SINGLETON(ClassName)            \
  static ClassName* Singleton() {          \
    static ClassName* ptr = new ClassName; \
    return ptr;                            \
  }

template<typename T>
bool operator==(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs) {
  return lhs.lock().get() == rhs.lock().get();
}

template<typename Key, typename T, typename Hash = std::hash<Key>>
using HashMap = std::unordered_map<Key, T, Hash>;

template<typename Key, typename Hash = std::hash<Key>>
using HashSet = std::unordered_set<Key, Hash>;

template<typename T, typename... Args>
std::unique_ptr<T> of_make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
void SortAndRemoveDuplication(std::vector<T>* vec) {
  std::sort(vec->begin(), vec->end());
  auto unique_it = std::unique(vec->begin(), vec->end());
  vec->erase(unique_it, vec->end());
}

inline std::string NewUniqueId() {
  static int64_t id = 0;
  return std::to_string(id++);
}

inline std::string LogDir() {
  static std::string log_dir = std::getenv("GLOG_log_dir");
  return log_dir;
}

inline void str_replace(std::string* str, char old_ch, char new_ch) {
  for (size_t i = 0; i < str->size(); ++i) {
    if (str->at(i) == old_ch) { str->at(i) = new_ch; }
  }
}

template<typename K, typename V>
void EraseIf(HashMap<K, V>* hash_map,
             std::function<bool(typename HashMap<K, V>::iterator)> cond) {
  for (auto it = hash_map->begin(); it != hash_map->end();) {
    if (cond(it)) {
      hash_map->erase(it++);
    } else {
      ++it;
    }
  }
}

#define OF_DECLARE_ENUM_TO_OSTREAM_FUNC(EnumType) \
  std::ostream& operator<<(std::ostream& out_stream, const EnumType&)

#define OF_DEFINE_ENUM_TO_OSTREAM_FUNC(EnumType)                          \
  std::ostream& operator<<(std::ostream& out_stream, const EnumType& x) { \
    out_stream << static_cast<int>(x);                                    \
    return out_stream;                                                    \
  }

template<typename OutType, typename InType>
OutType oneflow_cast(const InType&);

void Split(const std::string& text, const std::string& delims,
           std::function<void(std::string&&)> Func);

template<typename T>
void SplitAndParseAs(const std::string& text, const std::string& delims,
                     std::function<void(T&&)> Func) {
  Split(text, delims, [&Func](std::string&& s) { Func(oneflow_cast<T>(s)); });
}

inline uint32_t NewRandomSeed() {
  static std::mt19937 gen{std::random_device{}()};
  return gen();
}

}  // namespace oneflow

#endif  // ONEFLOW_CORE_COMMON_UTIL_H_
