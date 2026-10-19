#include <gtest/gtest.h>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "image_scaler.hpp"

namespace fs = std::filesystem;

class ScalerTest : public ::testing::Test {
  protected:
    fs::path root;
    fs::path input;
    fs::path output;

    void SetUp() override {
      root = fs::temp_directory_path() / "panelpress_scaler_tests";
      fs::remove_all(root);
      input = root / "input";
      output = root / "output";
      fs::create_directories(input);
      fs::create_directories(output);
    }

    void TearDown() override {
      fs::remove_all(root);
    }

    fs::path make_page(int width, int height) {
      fs::path page = input / "page.png";
      cv::imwrite(page.string(), cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 100, 200)));
      return page;
    }

    cv::Mat read_output() {
      return cv::imread((output / "page.png").string());
    }
};

TEST_F(ScalerTest, ScalesDownToMaxWidth) {
  ImageScaler().scale_img(make_page(2000, 3000), output, 1000, 0);

  cv::Mat result = read_output();
  EXPECT_EQ(result.cols, 1000);
  EXPECT_EQ(result.rows, 1500);
}

TEST_F(ScalerTest, ScalesDownToMaxHeight) {
  ImageScaler().scale_img(make_page(2000, 3000), output, 0, 1500);

  cv::Mat result = read_output();
  EXPECT_EQ(result.cols, 1000);
  EXPECT_EQ(result.rows, 1500);
}

// A 2:3 page on a 1264x1680 screen runs out of height first
TEST_F(ScalerTest, FitsInsideBothLimits) {
  ImageScaler().scale_img(make_page(2000, 3000), output, 1264, 1680);

  cv::Mat result = read_output();
  EXPECT_EQ(result.cols, 1120);
  EXPECT_EQ(result.rows, 1680);
}

// A double-page spread runs out of width first
TEST_F(ScalerTest, FitsWideSpreadsInsideBothLimits) {
  ImageScaler().scale_img(make_page(4000, 3000), output, 1264, 1680);

  cv::Mat result = read_output();
  EXPECT_EQ(result.cols, 1264);
  EXPECT_EQ(result.rows, 948);
}

TEST_F(ScalerTest, LeavesPagesThatAlreadyFitUntouched) {
  fs::path page = make_page(800, 1200);

  ImageScaler().scale_img(page, output, 1000, 1500);

  EXPECT_EQ(read_output().cols, 800);
  EXPECT_EQ(fs::file_size(output / "page.png"), fs::file_size(page));
}

TEST_F(ScalerTest, LeavesPagesExactlyAtTheLimitUntouched) {
  fs::path page = make_page(1000, 1500);

  ImageScaler().scale_img(page, output, 1000, 1500);

  EXPECT_EQ(fs::file_size(output / "page.png"), fs::file_size(page));
}

TEST_F(ScalerTest, NoLimitsLeavesPagesUntouched) {
  fs::path page = make_page(4000, 6000);

  ImageScaler().scale_img(page, output, 0, 0);

  EXPECT_EQ(fs::file_size(output / "page.png"), fs::file_size(page));
}
