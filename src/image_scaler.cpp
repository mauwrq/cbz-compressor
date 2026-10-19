#include "image_scaler.hpp"
#include <algorithm>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

void ImageScaler::scale_img(fs::path img_path, fs::path output_path, int max_width, int max_height) {
  cv::Mat img_in = cv::imread(img_path.string());
  fs::path out_path = output_path / img_path.filename();

  // fit the page inside max_width x max_height, where 0 means no limit in that direction
  double scaling_factor = 1.0;
  if (max_width > 0) {
    scaling_factor = std::min(scaling_factor, static_cast<double>(max_width) / img_in.cols);
  }
  if (max_height > 0) {
    scaling_factor = std::min(scaling_factor, static_cast<double>(max_height) / img_in.rows);
  }

  // scaling up only makes the file bigger and blurrier, so pages that already fit are copied as-is
  if (scaling_factor >= 1.0) {
    fs::copy_file(img_path, out_path, fs::copy_options::overwrite_existing);
    return;
  }
  cv::Mat img_out;
  cv::resize(img_in, img_out, cv::Size(), scaling_factor, scaling_factor, cv::INTER_AREA);
  cv::imwrite(out_path.string(), img_out);
}

void ImageScaler::scale_imgs(fs::path folder_path, fs::path output_path, int max_width, int max_height, ProgressBar& bar) {
  bar.start(std::distance(fs::directory_iterator{folder_path}, fs::directory_iterator{}));
  for (auto const& img : fs::directory_iterator{folder_path}) {
    scale_img(img.path(), output_path, max_width, max_height);
    bar.tick();
  }
  bar.finish();
}
