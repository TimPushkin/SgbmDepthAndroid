#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "../include/DepthEstimator.h"

constexpr int kCliArgsNum = 4;

inline std::vector<char> streamToVector(std::ifstream &s) {
    return {std::istreambuf_iterator<char>(s), std::istreambuf_iterator<char>()};
}

int main(int argc, char *argv[]) {
    if (argc != kCliArgsNum) {
        std::cerr << "Illegal CLI arguments num. Expected " << kCliArgsNum << "(including the command name) but was "
                  << argc;
        return 1;
    }

    auto *calibPath = argv[1];
    auto *leftImgPath = argv[2];
    auto *rightImgPath = argv[3];

    std::ifstream lImgStream(leftImgPath);
    std::ifstream rImgStream(rightImgPath);

    auto lImg = streamToVector(lImgStream);
    auto rImg = streamToVector(rImgStream);

    DepthEstimator depthEstimator(calibPath);
    std::vector<float> depth = depthEstimator.estimateDepth(lImg, rImg);

    for (const auto i : depth) {
        std::cout << i << ' ';
    }
}
