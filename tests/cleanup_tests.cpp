#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <zip.h>
#include <opencv2/opencv.hpp>
#include "cbz_tools.hpp"
#include "compressor.hpp"
#include "path_tools.hpp"

namespace fs = std::filesystem;

// Each test gets a fresh scratch folder with its own tmp/ and output/,
// plus a 3 page sample.cbz to compress.
class CleanupTest : public ::testing::Test {
  protected:
    fs::path root;
    fs::path sample_cbz;
    PathTools::project_paths project_paths;

    void SetUp() override {
      root = fs::temp_directory_path() / "panelpress_tests";
      fs::remove_all(root);
      project_paths.tmp = root / "tmp";
      project_paths.output = root / "output";
      fs::create_directories(project_paths.tmp);
      fs::create_directories(project_paths.output);

      fs::path pages = root / "pages";
      fs::create_directories(pages);
      for (int i = 0; i < 3; i++) {
        cv::Mat page(2400, 1600, CV_8UC3, cv::Scalar(i * 40, 100, 200));
        cv::imwrite((pages / ("page0" + std::to_string(i) + ".png")).string(), page);
      }
      CBZTools().rezip(pages, root, "sample.cbz");
      sample_cbz = root / "sample.cbz";
    }

    void TearDown() override {
      fs::remove_all(root);
    }

    static int count_entries(fs::path zip_path) {
      int err = 0;
      zip *archive = zip_open(zip_path.string().c_str(), ZIP_RDONLY, &err);
      if (archive == NULL) {
        return -1;
      }
      int count = zip_get_num_entries(archive, 0);
      zip_close(archive);
      return count;
    }
};

TEST_F(CleanupTest, InitTmpPathsCreatesStageFolders) {
  PathTools::tmp_paths tmp_paths = PathTools().init_tmp_paths(sample_cbz.string(), project_paths);

  EXPECT_EQ(tmp_paths.root, project_paths.tmp / "sample.cbz");
  EXPECT_TRUE(fs::is_directory(tmp_paths.unzipped));
  EXPECT_TRUE(fs::is_directory(tmp_paths.scaled));
  EXPECT_TRUE(fs::is_directory(tmp_paths.converted));
}

TEST_F(CleanupTest, InitTmpPathsClearsLeftoversFromPreviousRun) {
  fs::path stale = project_paths.tmp / "sample.cbz" / "converted" / "stale.webp";
  fs::create_directories(stale.parent_path());
  std::ofstream(stale) << "junk";

  PathTools().init_tmp_paths(sample_cbz.string(), project_paths);

  EXPECT_FALSE(fs::exists(stale));
}

TEST_F(CleanupTest, CompressRemovesTmpFolderWhenDone) {
  Compressor().compress(sample_cbz.string(), project_paths);

  EXPECT_TRUE(fs::exists(project_paths.output / "sample.cbz"));
  EXPECT_FALSE(fs::exists(project_paths.tmp / "sample.cbz"));
}

TEST_F(CleanupTest, CompressLeavesOtherComicsTmpFoldersAlone) {
  fs::path other = project_paths.tmp / "other.cbz" / "unzipped" / "page00.png";
  fs::create_directories(other.parent_path());
  std::ofstream(other) << "in use";

  Compressor().compress(sample_cbz.string(), project_paths);

  EXPECT_TRUE(fs::exists(other));
}

TEST_F(CleanupTest, LeftoverPagesDontEndUpInOutput) {
  fs::path stale = project_paths.tmp / "sample.cbz" / "converted" / "stale.webp";
  fs::create_directories(stale.parent_path());
  std::ofstream(stale) << "junk";

  Compressor().compress(sample_cbz.string(), project_paths);

  EXPECT_EQ(count_entries(project_paths.output / "sample.cbz"), 3);
}

TEST_F(CleanupTest, RunningTwiceGivesTheSameOutput) {
  Compressor().compress(sample_cbz.string(), project_paths);
  Compressor().compress(sample_cbz.string(), project_paths);

  EXPECT_EQ(count_entries(project_paths.output / "sample.cbz"), 3);
  EXPECT_FALSE(fs::exists(project_paths.tmp / "sample.cbz"));
}
