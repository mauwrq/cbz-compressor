#include <path_tools.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

PathTools::project_paths PathTools::init_project_paths(fs::path cbz_path) {
  PathTools::project_paths project_paths;
  project_paths.output = fs::absolute(cbz_path).parent_path() / "compressed";
  fs::create_directories(project_paths.output);
  project_paths.tmp = fs::temp_directory_path() / "cbz-compressor";
  fs::create_directories(project_paths.tmp);
  return project_paths;
};
  
PathTools::tmp_paths PathTools::init_tmp_paths(std::string cbz_name_str, PathTools::project_paths project_paths) {
  fs::path cbz_name = cbz_name_str;
  PathTools::tmp_paths tmp_paths;
  fs::path tmp_cbz_path = project_paths.tmp / cbz_name.filename();
  fs::remove_all(tmp_cbz_path);
  fs::create_directory(tmp_cbz_path);
  tmp_paths.root = tmp_cbz_path;
  fs::create_directory(tmp_cbz_path / "unzipped");
  tmp_paths.unzipped = tmp_cbz_path / "unzipped";
  fs::create_directory(tmp_cbz_path / "scaled");
  tmp_paths.scaled = tmp_cbz_path / "scaled";
  fs::create_directory(tmp_cbz_path / "converted");
  tmp_paths.converted = tmp_cbz_path / "converted";
  return tmp_paths;
};