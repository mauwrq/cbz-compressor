#pragma once
#include <filesystem>
#include <iostream>
#include <vector>

class ArgumentParser {
  public:
    struct args {
      std::vector<std::string> input;
      bool quiet = false;
      int width = 0;
    };
    args parse_args(int argc, char* argv[]);
};
