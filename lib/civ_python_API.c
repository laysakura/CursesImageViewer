#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "civ_errno.h"
#include "civ_color.h"
#include "civ_color_subtraction.h"
#include "civ_curses.h"

int
civ_color_subtraction_file2file(const char *in_img_path,
                                const char *out_img_path,
                                const civ_RGB *palette,
                                const int palette_len)
{
  IplImage *img = cvLoadImage(in_img_path, CV_LOAD_IMAGE_COLOR);
  assert(img);

#if defined DEBUG
  int i;
  printf("Palette:\n");
  for (i = 0; i < palette_len; ++i)
    printf("palette[%d] = (%d, %d, %d)\n", i, palette[i].r, palette[i].g, palette[i].b);
#endif

  color_subtraction_by_RGB_distance(img, palette, palette_len);

  /* Save color-subtracted image */
  assert(cvSaveImage(out_img_path, img, 0) == 1);

  /* Release object */
  cvReleaseImage(&img);

  return CIV_SUCCESS;
}

void
civ_curses_draw_img(const char *img_path,
                    const civ_RGB *palette,
                    const int palette_len)
{
  IplImage *img = cvLoadImage(img_path, CV_LOAD_IMAGE_COLOR);
  assert(img);

#if defined DEBUG
  int i;
  printf("Palette:\n");
  for (i = 0; i < palette_len; ++i)
    printf("palette[%d] = (%d, %d, %d)\n", i, palette[i].r, palette[i].g, palette[i].b);
#endif

  curses_draw_img(img, palette, palette_len);

  /* Draw on curses */
  /* draw_on_curses(img); */

  /* Release object */
  cvReleaseImage(&img);
}
