#pragma once
#include <string>

class ProgressBar {
  public:
    ProgressBar(std::string label, bool enabled);
    // show_percent prints "42%" instead of "42/100", for stages that aren't counted in pages
    void start(int total, bool show_percent = false);
    void set(int current);
    void tick();
    void finish();
    // A bar that never draws, used as the default when no bar is passed in.
    static ProgressBar& none();

  private:
    std::string label;
    bool enabled;
    int total = 0;
    int current = 0;
    bool show_percent = false;
    void draw();
};
