/*
 * utility.hpp
 *
 * デバッグ用のテンプレ関数の定義など
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! snipets 以外から include しない  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * header で src で include すると multiple definition のエラーになる
 * 
 * Create Date : 2019-12-08 20:22:59
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */

#ifndef DEBUG_UTILITY_HPP__
#define DEBUG_UTILITY_HPP__

#include <string>

#include <boost/filesystem.hpp>
#include <glog/logging.h>
#include <gperftools/profiler.h>


namespace fs = boost::filesystem;

namespace util {

inline void startLogging(int argc, char ** argv, std::string filename,
                         std::string save_dirname="./prof") {
    /*
     * google perftool で関数の速度などを測定するための設定と、
     * google logging で segmentation fault などが発生したときに stack trace を出力するための設定
     * Args :
     *    argc, argv : テストプログラムの argc, argv
     *    filename : テストプログラムのファイル名 (perftool の profile 保存のファイル名になる)
     *    save_dir : プロファイルを保存するディレクトリ
     */
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    fs::path save_dir(save_dirname);

    if (!fs::exists(save_dir)) {
        fs::create_directories(save_dir);
    }
    
    ProfilerStart(
        (save_dir / fs::path(fs::basename(fs::path(filename)) + ".prof")).generic_string().c_str());
}

inline void stopLogging() {
    ProfilerStop();
}

}

#endif // DEBUG_UTILITY_HPP__
