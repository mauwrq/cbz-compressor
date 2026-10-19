#pragma once
#include <string>
#include "path_tools.hpp"

class Compressor {
  public:
    // pages wider than this get scaled down to it, in pixels
    static constexpr int default_width = 1264;
    void compress(std::string cbz_input, PathTools::project_paths project_paths, bool show_progress = false, int width = default_width);
};
