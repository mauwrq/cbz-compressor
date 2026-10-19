#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>
#include <zip.h>
#include <opencv2/opencv.hpp>
#include "cbz_tools.hpp"

namespace fs = std::filesystem;

// Builds CBZs with whatever entry names a test needs, to mimic real-world archives.
class UnzipTest : public ::testing::Test {
  protected:
    fs::path root;
    fs::path unzipped;
    std::vector<uchar> png;

    void SetUp() override {
      root = fs::temp_directory_path() / "panelpress_unzip_tests";
      fs::remove_all(root);
      unzipped = root / "unzipped";
      fs::create_directories(unzipped);
      cv::imencode(".png", cv::Mat(200, 100, CV_8UC3, cv::Scalar(0, 100, 200)), png);
    }

    void TearDown() override {
      fs::remove_all(root);
    }

    // Each entry is a name and its contents. An empty string means "use the test PNG".
    fs::path make_cbz(std::vector<std::pair<std::string, std::string>> entries) {
      fs::path cbz = root / "test.cbz";
      int err = 0;
      zip *archive = zip_open(cbz.string().c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err);
      for (auto& [name, contents] : entries) {
        if (contents.empty()) {
          contents.assign(png.begin(), png.end());
        }
        zip_source_t *source = zip_source_buffer(archive, contents.data(), contents.size(), 0);
        zip_file_add(archive, name.c_str(), source, ZIP_FL_ENC_UTF_8);
      }
      zip_close(archive);
      return cbz;
    }

    std::vector<std::string> unzipped_names() {
      std::vector<std::string> names;
      for (auto const& file : fs::directory_iterator{unzipped}) {
        names.push_back(file.path().filename().string());
      }
      return names;
    }
};

TEST_F(UnzipTest, SkipsNonImageFiles) {
  fs::path cbz = make_cbz({
    {"ComicInfo.xml", "<ComicInfo><Title>Test</Title></ComicInfo>"},
    {"notes.txt", "hello"},
    {"page00.png", ""},
  });

  CBZTools().unzip(cbz.string(), unzipped);

  EXPECT_EQ(unzipped_names(), std::vector<std::string>{"page00.png"});
}

TEST_F(UnzipTest, ImageExtensionsAreCaseInsensitive) {
  fs::path cbz = make_cbz({{"PAGE00.PNG", ""}, {"page01.Jpg", ""}});

  CBZTools().unzip(cbz.string(), unzipped);

  EXPECT_EQ(unzipped_names().size(), 2);
}
