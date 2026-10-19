#pragma once
#include <filesystem>
#include "progress_bar.hpp"

class ImageScaler {
  public:
    // max_width / max_height of 0 means no limit in that direction
    void scale_img(std::filesystem::path img_path, std::filesystem::path output_path, int max_width, int max_height);
    void scale_imgs(std::filesystem::path folder_path, std::filesystem::path output_path, int max_width, int max_height, ProgressBar& bar = ProgressBar::none());
};
