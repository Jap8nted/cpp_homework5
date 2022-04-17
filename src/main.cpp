#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <sifts_internal.hpp>
#include <string>

int main() {
    float value = 0.343f;
    std::string filename = "test.bin";
    filename = "/home/stefania/projects/homework_5/data/lenna.png";
    const cv::Mat kInput = cv::imread(filename, cv::ImreadModes::IMREAD_GRAYSCALE);

    auto [descriptors, image_with_key] = ComputeSifts(filename);

    std::cout << "Descriptors" << descriptors.rows << "," << descriptors.cols << ","
              << descriptors.type() << "\n";
    std::vector images{kInput, image_with_key};

    cv::imshow("lena", image_with_key);
    cv::waitKey(0);
}
