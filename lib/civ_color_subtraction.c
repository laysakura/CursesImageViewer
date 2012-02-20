#include <opencv/cv.h>
#include "civ_color.h"
#include "civ_errno.h"
#include "civ_color_subtraction.h"



void
color_subtraction_by_RGB_distance(IplImage *img,
                                  const civ_RGB *palette,
                                  const int palette_len)
{
  int i, j;
  for (i = 0; i < img->height; ++i) {
    for (j = 0; j < img->width; ++j) {
      CvScalar pixel_color = cvGet2D(img, i, j);

      /* Find the closest color to pixel_color from palette */
      CvScalar new_pixel_color;
      nearest_RGB_from_palette(&pixel_color,
                               palette,
                               palette_len,
                               &new_pixel_color);

      /* Replace pixel with the closest color from palette */
      cvSet2D(img, i, j, new_pixel_color);
    }
  }
}

