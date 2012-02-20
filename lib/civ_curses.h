#ifndef _CIV_CURSES_H_
#define _CIV_CURSES_H_


#include <opencv/cv.h>
#include "civ_color.h"

void
curses_draw_img(const IplImage *img,
                const civ_RGB *palette,
                const int palette_len);


#endif /* _CIV_CURSES_H_ */
