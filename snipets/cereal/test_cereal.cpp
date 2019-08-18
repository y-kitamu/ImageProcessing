#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <Eigen/Eigen>
#include <boost/filesystem.hpp>
#include <ostream>
#include <istream>
#include "eigen.hpp"

namespace fs = boost::filesystem;

int main() {
    Eigen::Matrix3d identity = Eigen::Matrix3d::Identity();
    identity(0, 0) = 5;
    
    std::cout << identity << std::endl;

    fs::path save_dir("/home/kitamura/work/opencv/data/output/");

    if (!fs::exists(save_dir)) {
        const bool result = fs::create_directory(save_dir);
    }

    fs::path save_path = save_dir / fs::path("cereal_test.bin");

    {
        std::ofstream ofs(save_path.generic_string());
        cereal::BinaryOutputArchive ar(ofs);
        ar(identity);
    }

    {
        Eigen::MatrixXd output_id;
        std::ifstream ifs(save_path.generic_string());
        cereal::BinaryInputArchive ar(ifs);
        ar(output_id);
        std::cout << output_id << std::endl;
    }

}
