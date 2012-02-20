#ifndef _CIV_COLOR_SUBTRACTION_H_
#define _CIV_COLOR_SUBTRACTION_H_


void
color_subtraction_by_RGB_distance(IplImage *img_inout,
                                  const civ_RGB *palette,
                                  const int palette_len);


#endif /* _CIV_COLOR_SUBTRACTION_H_ */
