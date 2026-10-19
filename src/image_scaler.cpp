#include "image_scaler.hpp"
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

void ImageScaler::scale_img(fs::path img_path, fs::path output_path, double width) {
  cv::Mat img_in = cv::imread(img_path.string());
  fs::path out_path = output_path / img_path.filename();
  // scaling up only makes the file bigger and blurrier, so small pages are copied as-is
  if (img_in.cols <= width) {
    fs::copy_file(img_path, out_path, fs::copy_options::overwrite_existing);
    return;
  }
  cv::Mat img_out;
  double scaling_factor = width / img_in.cols;
  cv::resize(img_in, img_out, cv::Size(), scaling_factor, scaling_factor, cv::INTER_AREA);
  cv::imwrite(out_path.string(), img_out);
}

void ImageScaler::scale_imgs(fs::path folder_path, fs::path output_path, double width, ProgressBar& bar) {
  bar.start(std::distance(fs::directory_iterator{folder_path}, fs::directory_iterator{}));
  for (auto const& img : fs::directory_iterator{folder_path}) {
    scale_img(img.path(), output_path, width);
    bar.tick();
  }
  bar.finish();
}
