#include <iostream>
#include <boost/filesystem.hpp>
#include <gperftools/profiler.h>

namespace fs = boost::filesystem;

int func_a() {
    int g = 0;
    for (int i = 0; i < (int)1e5; i++) {
        g += i;
    }
    return g;
}

int func_b() {
    int g = 0;
    for (int i = 0; i < (int)4e5; i++) {
        g += i;
    }
    return g;
}


int main(int argc, char ** argv) {
    /*
     * Debug モードで compile -> 実行すると ProfilerStart() の引数で指定したファイルに
     * プロファイルが書き込まれる.terminal で
     * google-pprof <実行ファイル> <profiler file>
     * とすると、 pprof のコマンドラインが立ち上がる
     * callgrind とか出力形式をしていすると、結果が見れる。
     */
    
    std::cout << "hello gprof" << std::endl;

    fs::path prof_dir("./prof/");
    if (!fs::exists(prof_dir)) {
        fs::create_directory(prof_dir);
    }

    ProfilerStart((prof_dir / fs::path("test_google_profiler.prof")).generic_string().c_str());

    for (int i = 0; i < (int)1e5; i++) {
        func_a();
        func_b();

        int g = 0;
        for (int i = 0; i < (int)1e5; i++) {
            g += i;
        }
    }

    ProfilerStop();
 }
