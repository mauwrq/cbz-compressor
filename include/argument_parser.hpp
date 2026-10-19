#pragma once
#include <filesystem>
#include <iostream>
#include <vector>

class ArgumentParser {
  public:
    struct args {
      std::vector<std::string> input;
      bool quiet = false;
    };
    args parse_args(int argc, char* argv[]);
};
