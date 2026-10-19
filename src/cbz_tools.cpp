#include "cbz_tools.hpp"
#include <zip.h>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void CBZTools::unzip(std::string zip_path_str, fs::path output_path, ProgressBar& bar) {
  const char* zip_path = zip_path_str.c_str();
  
  zip *zip;
  int err = 0;
  if ((zip = zip_open(zip_path, 0, &err)) == NULL) { // TODO: proper error checking
    return;
  }

  int image_count = zip_get_num_entries(zip, 0);
  struct zip_stat st;
  zip_stat_init(&st);

  bar.start(image_count);
  for (int i = 0; i < image_count; i++) {
    zip_stat_index(zip, i, 0, &st);
    char *contents = new char[st.size];
    zip_file *f = zip_fopen_index(zip, i, 0);
    zip_fread(f, contents, st.size);
    std::ofstream file(output_path / st.name, std::ios::binary);
    file.write(contents, st.size);
    file.close();
    bar.tick();
  }
  zip_close(zip);
  bar.finish();
}

// libzip does the actual compressing inside zip_close, and reports progress from 0.0 to 1.0
static void rezip_progress(zip *, double progress, void *bar) {
  static_cast<ProgressBar*>(bar)->set(static_cast<int>(progress * 100));
}

void CBZTools::rezip(fs::path input_path, fs::path output_path, std::string cbz_name_str, ProgressBar& bar) {
  fs::path cbz_name = cbz_name_str;
  fs::path output_zip = output_path / cbz_name.filename();
  zip *zip;
  int err = 0;
  if ((zip = zip_open(output_zip.string().c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err)) == NULL) {
    return;
  }
  for (auto const& img : fs::directory_iterator{input_path}) {
    zip_source_t *source = zip_source_file(zip, img.path().string().c_str(), 0, 0);
    zip_file_add(zip, img.path().filename().string().c_str(), source, ZIP_FL_ENC_GUESS);
  }
  bar.start(100, true);
  zip_register_progress_callback_with_state(zip, 0.01, rezip_progress, NULL, &bar);
  zip_close(zip);
  bar.finish();
}
