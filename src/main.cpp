#include <homework_5.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <serialize.hpp>
#include <sifts_internal.hpp>
#include <string>

std::string getImageType(int number) {
    // find type
    int imgTypeInt = number % 8;
    std::string imgTypeString;

    switch (imgTypeInt) {
        case 0:
            imgTypeString = "8U";
            break;
        case 1:
            imgTypeString = "8S";
            break;
        case 2:
            imgTypeString = "16U";
            break;
        case 3:
            imgTypeString = "16S";
            break;
        case 4:
            imgTypeString = "32S";
            break;
        case 5:
            imgTypeString = "32F";
            break;
        case 6:
            imgTypeString = "64F";
            break;
        default:
            break;
    }

    // find channel
    int channel = (number / 8) + 1;

    std::stringstream type;
    type << "CV_" << imgTypeString << "C" << channel;

    return type.str();
}

/**
 * 1. Cast any type to uint_8t byte (char )
 * 2. Given the endiannes selected swap or not
 * 3. Write the bytes in the given order
 *
 *
 * @return int
 */

int test_function() {
    float value = 0.343f;
    std::string filename = "test.bin";
    std::ofstream fout(filename, std::ios_base::binary);
    fout.write((char *)&value, sizeof(value));
    fout.close();
    float my_value = 0.34343;
    std::ofstream fout1("secondtest.bin", std::ios_base::binary);
    ipb::serialization::WriteBinaryToFile(fout1, ipb::serialization::ByteOrder::BIG, my_value);

    fout1.close();
    filename = "/home/stefania/projects/homework_5/data/lenna.png";
    std::filesystem::path path(filename);
    std::cout << path.stem() << "what" << path.parent_path() << "\n";

    const cv::Mat kInput = cv::imread(filename, cv::ImreadModes::IMREAD_GRAYSCALE);

    auto [descriptors, image_with_key] = ComputeSifts(filename);

    ipb::serialization::Serialize(kInput, "/home/stefania/projects/homework_5/data/lenna.bin");

    std::cout << "Descriptors" << descriptors.rows << "," << descriptors.cols << ","
              << getImageType(descriptors.type()) << "\n";
    std::cout << "image" << kInput.rows << "," << kInput.cols << "," << getImageType(kInput.type())
              << "\n";
    std::vector images{kInput, image_with_key};

    cv::imshow("lena", image_with_key);
    cv::waitKey(0);

    cv::Mat deserialized =
        ipb::serialization::Deserialize("/home/stefania/projects/homework_5/data/lenna.bin");

    cv::imshow("lena2", deserialized);

    cv::waitKey(0);

    std::filesystem::path img_path("/home/stefania/projects/homework_5/data/freiburg/images");

    ipb::serialization::sifts::ConvertDataset(img_path);

    std::filesystem::path bin_path("/home/stefania/projects/homework_5/data/freiburg/bin/");
    ipb::serialization::sifts::LoadDataset(bin_path);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Pass a valid path, just one attribute is required" << std::endl;
        return 0;
    }
    std::string data_path(argv[1]);
    std::filesystem::path images_path(data_path);
    ipb::serialization::sifts::ConvertDataset(images_path);
    return 0;
}
