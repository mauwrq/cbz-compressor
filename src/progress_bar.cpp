#include "progress_bar.hpp"
#include <cstdio>
#include <string>

ProgressBar::ProgressBar(std::string label, bool enabled) : label(label), enabled(enabled) {}

void ProgressBar::start(int total, bool show_percent) {
  this->total = total;
  this->show_percent = show_percent;
  current = 0;
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
    std::fputc('\n', stderr);
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
  if (show_percent) {
    int percent = total > 0 ? current * 100 / total : 100;
    std::fprintf(stderr, "\r%-11s [%s] %d%%", label.c_str(), bar.c_str(), percent);
  }
  else {
    std::fprintf(stderr, "\r%-11s [%s] %d/%d", label.c_str(), bar.c_str(), current, total);
  }
  std::fflush(stderr);
}
