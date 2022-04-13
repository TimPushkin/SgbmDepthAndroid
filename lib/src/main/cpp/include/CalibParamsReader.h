#ifndef SGBM_ANDROID_CALIBPARAMSREADER_H
#define SGBM_ANDROID_CALIBPARAMSREADER_H

#include <memory>

struct CalibParams {
    int rows;
    int cols;
    struct {
        std::unique_ptr<unsigned char> x;
        std::unique_ptr<unsigned char> y;
    } leftMap;
    struct {
        std::unique_ptr<unsigned char> x;
        std::unique_ptr<unsigned char> y;
    } rightMap;
    std::unique_ptr<unsigned char> Q;
};

class CalibParamsReader {
public:
    static CalibParams readCalibParams(const char *content,
                                       const std::string &leftMapXName = "leftMapX",
                                       const std::string &leftMapYName = "leftMapY",
                                       const std::string &rightMapXName = "rightMapX",
                                       const std::string &rightMapYName = "rightMapY",
                                       const std::string &qName = "Q");
};

#endif
