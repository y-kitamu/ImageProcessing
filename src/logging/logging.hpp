/*
 * logging.hpp
 *
 *
 * Create Date : 2020-05-06 01:07:32
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef LOGGING_HPP__
#define LOGGING_HPP__

#include <iostream>
#include <chrono>
#include <ctime>
#include <regex>

#include <fmt/format.h>

namespace logging {

#ifndef logd
#define logd(...) logging::_logd(__FILE__, __LINE__, __VA_ARGS__)
#endif

template <typename S, typename... Args>
void _logd(std::string filename, int line, const S& format_str, const Args&... args) {
    /*
     * debug log
     */
    static const std::regex re(".*\n");

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    char mbstr[20];
    std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));

    fmt::print("[{} ({} at line {})] ", mbstr, filename, line);
    fmt::print(format_str, args...);
    if (!std::regex_match(format_str, re)) {
        std::cout << std::endl;
    }
}

} // namespace


#endif // LOGGING_HPP__
