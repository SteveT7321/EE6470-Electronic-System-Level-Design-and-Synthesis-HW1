#include <cmath>

#include "mmfilter.h"

mmfilter::mmfilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// mean_mask
const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}}; // sum = 10
/*const int mask[MASK_N][MASK_X][MASK_Y] = {{{1/9, 1/9, 1/9}, 
                                            {1/9, 1/9, 1/9}, 
                                            {1/9, 1/9, 1/9}}}; */

void mmfilter::do_filter() {
  while (true) {
    std::vector<unsigned char> pixels_r;
    std::vector<unsigned char> pixels_g;
    std::vector<unsigned char> pixels_b;

    // Read in a 3x3 sliding window of pixels centered around the current pixel
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        unsigned char r = i_r.read();
        unsigned char g = i_g.read();
        unsigned char b = i_b.read();
        pixels_r.push_back(r);
        pixels_g.push_back(g);
        pixels_b.push_back(b);
      }
    }

    // 1. Applying "median filter" to each color channel
    std::sort(pixels_r.begin(), pixels_r.end());
    std::sort(pixels_g.begin(), pixels_g.end());
    std::sort(pixels_b.begin(), pixels_b.end());
    unsigned char filtered_r = pixels_r[pixels_r.size() / 2];
    unsigned char filtered_g = pixels_g[pixels_g.size() / 2];
    unsigned char filtered_b = pixels_b[pixels_b.size() / 2];

    // 2. Applying "mean filter" to each color channel
    int sum_r = 0, sum_g = 0, sum_b = 0;

    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        sum_r += filtered_r * mask[u][v];
        sum_g += filtered_g * mask[u][v];
        sum_b += filtered_b * mask[u][v];
      }
    }

    filtered_r = sum_r / 10;
    filtered_g = sum_g / 10;
    filtered_b = sum_b / 10;

    // Write out the filtered pixel
    o_result_r.write(filtered_r);
    o_result_g.write(filtered_g);
    o_result_b.write(filtered_b);

    wait(5); //emulate module delay
  }
}
