/*
 * local_mapping.hpp
 *
 *
 * Create Date : 2020-05-03 14:23:12
 * Copyright (c) 2019- Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef LOCAL_MAPPING_HPP__
#define LOCAL_MAPPING_HPP__

#include <iostream>
#include <sstream>

#include <future>


namespace slam {

class LocalMapping {
  public:
    void run() {
        std::stringstream ss;
        ss << "local mapping thread : " << std::this_thread::get_id() << std::endl;
        std::cout << ss.str();
    }
};

} // namespace slam


#endif // LOCAL_MAPPING_HPP__
