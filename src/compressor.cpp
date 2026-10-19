#include "compressor.hpp"
#include <filesystem>
#include "cbz_tools.hpp"
#include "image_scaler.hpp"
#include "image_converter.hpp"
#include "path_tools.hpp"

namespace fs = std::filesystem;

void Compressor::compress(std::string cbz_input, PathTools::project_paths project_paths) {
  PathTools path_tools;
  CBZTools cbz_tools;
  ImageScaler scaler;
  ImageConverter converter;

  PathTools::tmp_paths tmp_paths = path_tools.init_tmp_paths(cbz_input, project_paths);
  cbz_tools.unzip(cbz_input, tmp_paths.unzipped);
  scaler.scale_imgs(tmp_paths.unzipped, tmp_paths.scaled, 1264);
  fs::remove_all(tmp_paths.unzipped);
  converter.to_webps(tmp_paths.scaled, tmp_paths.converted);
  fs::remove_all(tmp_paths.scaled);
  cbz_tools.rezip(tmp_paths.converted, project_paths.output, cbz_input);
  fs::remove_all(tmp_paths.root);
}