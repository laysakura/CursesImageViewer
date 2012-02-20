#ifndef _CIV_CURSES_H_
#define _CIV_CURSES_H_


#include <opencv/cv.h>
#include "civ_color.h"

void
curses_draw_img(IplImage *img,
                const civ_RGB *palette,
                const int palette_len);

void
get_curses_palette(civ_RGB *palette_out);

void
civ_curses_draw_img(const char *img_path,
                    const civ_RGB *palette,
                    const int palette_len);


#endif /* _CIV_CURSES_H_ */
