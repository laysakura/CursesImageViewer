#ifndef _CIV_COLOR_H_
#define _CIV_COLOR_H_


#include <opencv/cv.h>


typedef struct civ_RGB {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} civ_RGB;

/* civ_RGB[] is called palette.

   RGB -> curses_color_number:
     Linear-search palette

   curses_color_number -> RGB:
     palette[color_number].(r|g|b)
*/


static inline void
CvScalar_to_RGB(const CvScalar *cvScalar, civ_RGB *RGB_out)
{
  RGB_out->r = cvScalar->val[2];
  RGB_out->g = cvScalar->val[1];
  RGB_out->b = cvScalar->val[0];
}

#include <stdio.h>

static inline int
CvScalar_to_color_number(const CvScalar *cvScalar,
                         const civ_RGB *palette,
                         const int palette_len)
{
  civ_RGB rgb;
  CvScalar_to_RGB(cvScalar, &rgb);

  int color_number;
  for (color_number = 0; color_number < palette_len; ++color_number) {
    if (abs(palette[color_number].r - rgb.r) <= 1.0 &&
        abs(palette[color_number].g - rgb.g) <= 1.0 &&
        abs(palette[color_number].b - rgb.b) <= 1.0) {
      FILE *f = fopen("test.txt", "a");
      fprintf(f, "chosen_palette=(%d,%d,%d) ; pixel=(%d,%d,%d)\n",
              palette[color_number].r, palette[color_number].g, palette[color_number].b,
              rgb.r, rgb.g, rgb.b);
      fclose(f);
      return color_number;
    }
  }
  /* cvScalar is not in the palette */
  abort();
}

static inline float
RGB_distance_between(const CvScalar *v1, const CvScalar *v2)
{
  return (v1->val[0] - v2->val[0]) * (v1->val[0] - v2->val[0]) +
    (v1->val[1] - v2->val[1]) * (v1->val[1] - v2->val[1]) +
    (v1->val[2] - v2->val[2]) * (v1->val[2] - v2->val[2]);
}


static inline void
nearest_RGB_and_color_number_from_palette(const CvScalar *pixel_color,
                                          const civ_RGB *palette,
                                          const int palette_len,
                                          CvScalar *nearest_RGB_from_palette,
                                          int *nearest_color_number_from_palette)
{
  /* Indices of palette are assumed to be the same as color_number */
  float min_distance = 255*255 + 255*255 + 255*255;
  int color_number_with_min_distance = -1;
  int color_number;
  for (color_number = 0; color_number < palette_len; ++color_number) {
    CvScalar palette_color = CV_RGB(palette[color_number].r,
                                    palette[color_number].g,
                                    palette[color_number].b);
    float distance = RGB_distance_between(pixel_color, &palette_color);
    if (distance < min_distance) {
      min_distance = distance;
      color_number_with_min_distance = color_number;
    }
  }

  /* Result */
  *nearest_RGB_from_palette = CV_RGB(palette[color_number_with_min_distance].r,
                                     palette[color_number_with_min_distance].g,
                                     palette[color_number_with_min_distance].b);
  *nearest_color_number_from_palette = color_number_with_min_distance;
}

static inline void
nearest_RGB_from_palette(const CvScalar *pixel_color,
                         const civ_RGB *palette,
                         const int palette_len,
                         CvScalar *nearest_RGB_from_palette)
{
  int x;
  nearest_RGB_and_color_number_from_palette(pixel_color,
                                            palette,
                                            palette_len,
                                            nearest_RGB_from_palette,
                                            &x);
}

static inline void
nearest_color_number_from_palette(const CvScalar *pixel_color,
                                  const civ_RGB *palette,
                                  const int palette_len,
                                  int *nearest_color_number_from_palette)
{
  CvScalar x;
  nearest_RGB_and_color_number_from_palette(pixel_color,
                                            palette,
                                            palette_len,
                                            &x,
                                            nearest_color_number_from_palette);
}


#endif /* _CIV_COLOR_H_ */
