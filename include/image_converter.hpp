#pragma once
#include <filesystem>
#include "progress_bar.hpp"

class ImageConverter {
  public:
    void to_jpg(std::filesystem::path img_path, std::filesystem::path output_path);
    void to_jpgs(std::filesystem::path folder_path, std::filesystem::path output_path);
    void to_webp(std::filesystem::path img_path, std::filesystem::path output_path);
    void to_webps(std::filesystem::path folder_path, std::filesystem::path output_path, ProgressBar& bar = ProgressBar::none());
};
