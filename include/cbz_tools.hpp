#pragma once
#include <filesystem>
#include <string>
#include "progress_bar.hpp"

class CBZTools {
  public:
    void unzip(std::string zip_path_str, std::filesystem::path output_path, ProgressBar& bar = ProgressBar::none());
    void rezip(std::filesystem::path input_path, std::filesystem::path output_path, std::string cbz_name, ProgressBar& bar = ProgressBar::none());
};
