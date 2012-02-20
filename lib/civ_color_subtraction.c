#include <opencv/cv.h>
#include "civ_color.h"
#include "civ_errno.h"
#include "civ_color_subtraction.h"


void
color_subtraction_by_RGB_distance(IplImage *img_inout,
                                  const civ_RGB *palette,
                                  const int palette_len)
{
  int i, j;
  for (i = 0; i < img_inout->height; ++i) {
    for (j = 0; j < img_inout->width; ++j) {
      CvScalar pixel_color = cvGet2D(img_inout, i, j);

      /* Find the closest color to pixel_color from palette */
      float min_distance = 255*255 + 255*255 + 255*255;
      int min_distance_idx = -1;
      int distance_idx;
      for (distance_idx = 0; distance_idx < palette_len; ++distance_idx) {
        CvScalar palette_color = CV_RGB(palette[distance_idx].r,
                                        palette[distance_idx].g,
                                        palette[distance_idx].b);
        float distance = RGB_distance_between(&pixel_color, &palette_color);
        if (distance < min_distance) {
          min_distance = distance;
          min_distance_idx = distance_idx;
        }
      }
      CvScalar new_pixel_color = CV_RGB(palette[min_distance_idx].r,
                                        palette[min_distance_idx].g,
                                        palette[min_distance_idx].b);

      /* Replace pixel with the closest color from palette */
      cvSet2D(img_inout, i, j, new_pixel_color);
    }
  }
}

