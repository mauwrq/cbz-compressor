#include <filesystem>
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

  PathTools::project_paths project_paths = path_tools.init_project_paths(fs::canonical(argv[0]));
  compressor.compress(cbz_input, project_paths);
}