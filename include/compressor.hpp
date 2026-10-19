#pragma once
#include <string>
#include "path_tools.hpp"

class Compressor {
  public:
    // pages are scaled down to fit inside max_width x max_height, where 0 means no limit
    void compress(std::string cbz_input, PathTools::project_paths project_paths, bool show_progress = false, int max_width = 0, int max_height = 0);
};
