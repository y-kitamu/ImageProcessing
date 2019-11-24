/*
 * test_google_glog.cpp
 *
 * Create Date : 2019-11-24 11:10:38
 * Copyright (c) 2019 Yusuke Kitamura <ymyk6602@gmail.com>
 */
#include <glog/logging.h>


void func() {
    throw;
}

int main(int argc, char ** argv) {
    /*
     * エラーが出たらスタックトレースを自動で出力する
     * 
     */
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    func();
}
