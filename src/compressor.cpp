#include "compressor.hpp"
#include <chrono>
#include <cstdio>
#include <filesystem>
#include "cbz_tools.hpp"
#include "image_scaler.hpp"
#include "image_converter.hpp"
#include "path_tools.hpp"

namespace fs = std::filesystem;

void Compressor::compress(std::string cbz_input, PathTools::project_paths project_paths, bool show_progress, int width) {
  PathTools path_tools;
  CBZTools cbz_tools;
  ImageScaler scaler;
  ImageConverter converter;
  ProgressBar unzip_bar("Unzipping", show_progress);
  ProgressBar scale_bar("Scaling", show_progress);
  ProgressBar convert_bar("Converting", show_progress);
  ProgressBar zip_bar("Zipping", show_progress);

  auto start_time = std::chrono::steady_clock::now();

  PathTools::tmp_paths tmp_paths = path_tools.init_tmp_paths(cbz_input, project_paths);
  cbz_tools.unzip(cbz_input, tmp_paths.unzipped, unzip_bar);
  scaler.scale_imgs(tmp_paths.unzipped, tmp_paths.scaled, width, scale_bar);
  fs::remove_all(tmp_paths.unzipped);
  converter.to_webps(tmp_paths.scaled, tmp_paths.converted, convert_bar);
  fs::remove_all(tmp_paths.scaled);
  int page_count = std::distance(fs::directory_iterator{tmp_paths.converted}, fs::directory_iterator{});
  cbz_tools.rezip(tmp_paths.converted, project_paths.output, cbz_input, zip_bar);
  fs::remove_all(tmp_paths.root);

  if (show_progress) {
    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start_time;
    fs::path output_cbz = project_paths.output / fs::path(cbz_input).filename();
    double input_mb = fs::file_size(cbz_input) / (1024.0 * 1024.0);
    double output_mb = fs::file_size(output_cbz) / (1024.0 * 1024.0);
    double change = (1.0 - output_mb / input_mb) * 100.0;

    std::fprintf(stderr, "Done: %s\n", output_cbz.string().c_str());
    std::fprintf(stderr, "Size: %.1f MB -> %.1f MB (%.0f%% %s)\n",
                 input_mb, output_mb, change >= 0 ? change : -change, change >= 0 ? "smaller" : "larger");
    std::fprintf(stderr, "Time: %.1fs (%.2fs per page)\n",
                 elapsed.count(), page_count > 0 ? elapsed.count() / page_count : 0.0);
  }
}