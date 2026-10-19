#pragma once
#include <string>
#include "path_tools.hpp"

class Compressor {
  public:
    void compress(std::string cbz_input, PathTools::project_paths project_paths);
};
