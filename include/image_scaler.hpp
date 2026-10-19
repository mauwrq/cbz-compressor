#pragma once
#include <filesystem>
#include "progress_bar.hpp"

class ImageScaler {
  public:
    void scale_img(std::filesystem::path img_path, std::filesystem::path output_path, double width);
    void scale_imgs(std::filesystem::path folder_path, std::filesystem::path output_path, double width, ProgressBar& bar = ProgressBar::none());
};
