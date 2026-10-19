#include <filesystem>
#include <iostream>
#include <string>
#include <argument_parser.hpp>
#include "compressor.hpp"
#include "path_tools.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  ArgumentParser arg_parser;
  PathTools path_tools;
  Compressor compressor;

  ArgumentParser::args args = arg_parser.parse_args(argc, argv);
  std::string cbz_input = args.input[0];
  if (args.width <= 0) {
    std::cerr << "error: --width must be a positive number of pixels\n";
    return 1;
  }

  PathTools::project_paths project_paths = path_tools.init_project_paths(cbz_input);
  compressor.compress(cbz_input, project_paths, !args.quiet, args.width);
}