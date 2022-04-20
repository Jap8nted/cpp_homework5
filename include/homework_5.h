#include <convert_dataset.hpp>
#include <filesystem>
#include <serialize.hpp>
#include <sifts_internal.hpp>

void ipb::serialization::Serialize(const cv::Mat &m, const std::string &filename) {
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

cv::Mat ipb::serialization::Deserialize(const std::string &filename) {
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

bool ipb::serialization::fileExists(const std::string &filename) {
    std::filesystem::path image_path(filename);
    bool exists = std::filesystem::exists(image_path);
    return exists;
}

void ipb::serialization::sifts::ConvertDataset(const std::filesystem::path &img_path) {
    if (!std::filesystem::exists(img_path)) {
        std::cout << "This path does not exists\n";
        return;
    }
    const auto &parent_path = img_path.parent_path();
    const auto &bin_path = parent_path / std::filesystem::path("bin");
    if (!std::filesystem::exists(bin_path)) {
        std::filesystem::create_directories(bin_path);
    };
    for (const auto &entry : std::filesystem::directory_iterator(img_path)) {
        const auto &stem = entry.path().stem().string();
        const auto &extension = entry.path().extension();
        std::string file_path(entry.path().string());
        if (extension == ".png") {
            auto [descriptors, image_with_key] = ComputeSifts(file_path);
            const auto &descriptors_filename = bin_path.string() + "/" + stem + ".bin";
            std::cout << "Processing : " << descriptors_filename << "\n";
            ipb::serialization::Serialize(descriptors, descriptors_filename);
        }
    }
}

std::vector<cv::Mat> ipb::serialization::sifts::LoadDataset(const std::filesystem::path &bin_path) {
    std::vector<cv::Mat> deserialized_images;
    if (!std::filesystem::exists(bin_path)) {
        std::cout << "This path does not exists\n";
        return deserialized_images;
    };

    for (const auto &entry : std::filesystem::directory_iterator(bin_path)) {
        const auto &stem = entry.path().stem().string();
        const auto &extension = entry.path().extension();
        if ((extension == ".bin") & (std::filesystem::exists(entry))) {
            std::cout << "Processing : " << stem << "\n";
            cv::Mat image_bin = ipb::serialization::Deserialize(entry.path().string());
            deserialized_images.push_back(image_bin);
        };
    }
    return deserialized_images;
}