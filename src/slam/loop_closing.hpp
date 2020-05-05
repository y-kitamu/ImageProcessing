/*
 * loop_closing.hpp
 *
 *
 * Create Date : 2020-05-03 14:23:20
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef LOOP_CLOSING_HPP__
#define LOOP_CLOSING_HPP__

#include <iostream>
#include <sstream>

#include <future>

namespace slam {

class LoopClosing {
  public:
    void run() {
        std::stringstream ss;
        ss << "Loop Closing thread : "  << std::this_thread::get_id() << std::endl;
        std::cout << ss.str();
    }
};

} // namespace slam


#endif // LOOP_CLOSING_HPP__
