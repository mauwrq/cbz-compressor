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
  if ((args.width && *args.width <= 0) || (args.height && *args.height <= 0)) {
    std::cerr << "error: --width and --height must be a positive number of pixels\n";
    return 1;
  }

  PathTools::project_paths project_paths = path_tools.init_project_paths(cbz_input);
  compressor.compress(cbz_input, project_paths, !args.quiet, args.width.value_or(0), args.height.value_or(0));
}