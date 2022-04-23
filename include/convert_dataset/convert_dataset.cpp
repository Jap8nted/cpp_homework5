#include <convert_dataset.hpp>
#include <filesystem>
#include <iostream>
#include <serialize.hpp>
#include <sifts_internal.hpp>

namespace ipb::serialization::sifts {
void ConvertDataset(const std::filesystem::path &img_path) {
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

std::vector<cv::Mat> LoadDataset(const std::filesystem::path &bin_path) {
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
}  // namespace ipb::serialization::sifts
