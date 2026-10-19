#pragma once
#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>

class ArgumentParser {
  public:
    struct args {
      std::vector<std::string> input;
      bool quiet = false;
      // empty when the flag wasn't given, which means no limit
      std::optional<int> width;
      std::optional<int> height;
    };
    args parse_args(int argc, char* argv[]);
};
