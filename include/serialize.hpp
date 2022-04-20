// @file      serialize.hpp
// @author    Ignacio Vizzo     [ivizzo@uni-bonn.de]
//
// Copyright (c) 2020 Ignacio Vizzo, all rights reserved
#ifndef SERIALIZE_HPP_
#define SERIALIZE_HPP_

#include <bitset>
#include <fstream>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <string>
namespace ipb::serialization {

/**
 * @brief Writes the input cv::Mat(Image, SIFT, etc) to a binary file
 *
 * @param m        The input cv::Mat you want to serialize.
 * @param filename The filename of the file where the cv::Mat will be written.
 */
void Serialize(const cv::Mat& m, const std::string& filename);

/**
 * @brief Reads a binary file representing a cv::Mat type and outputs the
 * contetnt of this file into a new cv::Mat
 *
 * @param filename The binary file you want to read
 * @return cv::Mat The cv::Mat created from the binary file
 */
cv::Mat Deserialize(const std::string& filename);

enum class ByteOrder { BIG, LITTLE };

/**
 * @brief serializes a unique value
 *
 * @tparam T
 * @param ostream
 * @param byte_order
 * @param value
 */
template <typename T>
void WriteBinaryToFile(std::ofstream& ostream, ByteOrder byte_order, T value) {
    // Check if ostream is valid and go for it, how to handle exceptions?

    uint8_t* byte_of_value = reinterpret_cast<uint8_t*>(&value);
    int size = sizeof(value);
    std::cout << value << ":";
    for (int i = 0; i < size; i++) {
        // +byte_of_value is a trick to treat it as a number
        std::bitset<8> x(byte_of_value[i]);
        std::cout << x << " ";
    }
    if (byte_order == ByteOrder::BIG) {
    } else if (byte_order == ByteOrder::LITTLE) {
    };

    ostream.write((char*)&value, size);
};

bool fileExists(const std::string & filename);

}  // namespace ipb::serialization

#endif  // SERIALIZE_HPP_
