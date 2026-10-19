#include "progress_bar.hpp"
#include <cstdio>
#include <string>

ProgressBar::ProgressBar(std::string label, bool enabled) : label(label), enabled(enabled) {}

void ProgressBar::start(int total, bool show_percent) {
  this->total = total;
  this->show_percent = show_percent;
  current = 0;
  start_time = std::chrono::steady_clock::now();
  draw();
}

void ProgressBar::set(int current) {
  if (current == this->current) {
    return;
  }
  this->current = current;
  draw();
}

void ProgressBar::tick() {
  set(current + 1);
}

void ProgressBar::finish() {
  set(total);
  if (enabled) {
    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start_time;
    std::fprintf(stderr, "  %.1fs\n", elapsed.count());
  }
}

ProgressBar& ProgressBar::none() {
  static ProgressBar bar("", false);
  return bar;
}

void ProgressBar::draw() {
  if (!enabled) {
    return;
  }
  const int width = 30;
  int filled = total > 0 ? current * width / total : width;
  std::string bar = std::string(filled, '#') + std::string(width - filled, '-');
  std::string count;
  if (show_percent) {
    int percent = total > 0 ? current * 100 / total : 100;
    count = std::to_string(percent) + "%";
  }
  else {
    count = std::to_string(current) + "/" + std::to_string(total);
  }
  // pad the count so the stage times line up
  std::fprintf(stderr, "\r%-11s [%s] %-7s", label.c_str(), bar.c_str(), count.c_str());
  std::fflush(stderr);
}
