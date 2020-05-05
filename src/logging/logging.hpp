/*
 * logging.hpp
 *
 *
 * Create Date : 2020-05-06 01:07:32
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef LOGGING_HPP__
#define LOGGING_HPP__

#include <chrono>
#include <ctime>
#include <fmt/format.h>


namespace logging {

template <typename S, typename... Args>
void logd(const S& format_str, const Args&... args) {
    /*
     * debug log
     */
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    char mbstr[19];
    std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
    fmt::print("[{}] ({} at line {}) ", std::string(mbstr), __FILE__, __LINE__);
    if (format_str[-1] == "\n") {
        fmt::print(format_str, args...);
    } else {
        fmt::print(format_str + "\n", args...);
    }
}

} // namespace


#endif // LOGGING_HPP__
