#include <filesystem>
#include <serialize.hpp>
#include <sifts_internal.hpp>

namespace ipb::serialization {
void Serialize(const cv::Mat &m, const std::string &filename) {
    std::ofstream fout(filename, std::ios_base::binary);
    /**
     * @brief
     * Header of binary file containing definition
     */
    int rows = m.rows;
    int columns = m.cols;
    int type = m.type();
    int channels = m.channels();
    int size_of_int = sizeof(int);
    fout.write(reinterpret_cast<char *>(&rows), size_of_int);
    fout.write(reinterpret_cast<char *>(&columns), size_of_int);
    fout.write(reinterpret_cast<char *>(&type), size_of_int);
    fout.write(reinterpret_cast<char *>(&channels), size_of_int);
    if (m.isContinuous()) {
        fout.write(m.ptr<char>(0), (m.dataend - m.datastart));
    } else {
        int rowsz = CV_ELEM_SIZE(type) * m.cols;
        for (int r = 0; r < m.rows; ++r) {
            fout.write(m.ptr<char>(r), rowsz);
        }
    }
}

cv::Mat Deserialize(const std::string &filename) {
    if (!ipb::serialization::fileExists(filename)) {
        std::cout << "This file does not exist\n";
        cv::Mat newMatrix;
        return newMatrix;
    }
    std::ifstream fs(filename, std::ios_base::binary);

    int rows, cols, type, channels;
    fs.read((char *)&rows, sizeof(int));      // rows
    fs.read((char *)&cols, sizeof(int));      // cols
    fs.read((char *)&type, sizeof(int));      // type
    fs.read((char *)&channels, sizeof(int));  // channels

    // Data
    cv::Mat mat(rows, cols, type);
    fs.read((char *)mat.data, CV_ELEM_SIZE(type) * rows * cols);

    return mat;
}

bool fileExists(const std::string &filename) {
    std::filesystem::path image_path(filename);
    bool exists = std::filesystem::exists(image_path);
    return exists;
}
}  // namespace ipb::serialization