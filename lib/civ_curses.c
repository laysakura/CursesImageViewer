#include <ncurses.h>
#include "civ_curses.h"
#include "civ_color.h"
#include "civ_color_subtraction.h"


/* NOTE that each color_pair == color_number + 1
   since color_pair is 1-origin and color_number 0-origin. */
#define color_number_to_color_pair(color_number) (color_number + 1)

void
curses_setup()
{
  /* Initialization */
  initscr();
  if(has_colors() == FALSE) {
    endwin();
    fprintf(stderr, "Your terminal does not support color\n");
    exit(1);
  }
  start_color();

  /* Color pair definition */
  init_pair(color_number_to_color_pair(COLOR_BLACK), COLOR_BLACK, COLOR_BLACK);
  init_pair(color_number_to_color_pair(COLOR_RED), COLOR_RED, COLOR_RED);
  init_pair(color_number_to_color_pair(COLOR_GREEN), COLOR_GREEN, COLOR_GREEN);
  init_pair(color_number_to_color_pair(COLOR_YELLOW), COLOR_YELLOW, COLOR_YELLOW);
  init_pair(color_number_to_color_pair(COLOR_BLUE), COLOR_BLUE, COLOR_BLUE);
  init_pair(color_number_to_color_pair(COLOR_MAGENTA), COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(color_number_to_color_pair(COLOR_CYAN), COLOR_CYAN, COLOR_CYAN);
  init_pair(color_number_to_color_pair(COLOR_WHITE), COLOR_WHITE, COLOR_WHITE);
}

void
curses_teardown()
{
  endwin();
}


void
curses_draw_pixel(int i, int j, const int color_number)
{
  attron(COLOR_PAIR(color_number_to_color_pair(color_number)));
  mvaddch(i, j, ' ');
}

void
curses_draw_img_with_origin(const IplImage *img,
                            const civ_RGB *palette,
                            const int palette_len,
                            const int origin_i,
                            const int origin_j)
{
  int img_height = img->height;
  int img_width = img->width;
  int win_height, win_width;
  getmaxyx(stdscr, win_height, win_width);

  /* Draw only within terminal size */
  int i, j;
  for (i = 0; i < win_height ; ++i) {
    for (j = 0; j < win_width; ++j) {
      /* Point is within image */
      if (0 <= origin_i + i && origin_i + i < img_height &&
          0 <= origin_j + j && origin_j + j < img_width) {
        CvScalar pixel_color = cvGet2D(img, origin_i + i, origin_j + j);
        int color_number = CvScalar_to_color_number(&pixel_color,
                                                    palette,
                                                    palette_len);
        curses_draw_pixel(i, j, color_number);
      }
      /* Point is out of image */
      else {
        curses_draw_pixel(i, j, COLOR_BLACK);
      }
    }
  }
  refresh();
}


void
curses_draw_img(IplImage *img,
                const civ_RGB *palette,
                const int palette_len)
{
  /* Subtract color */
  color_subtraction_by_RGB_distance(img, palette, palette_len);

  /* Setup curses */
  curses_setup();

  int origin_i = 0;
  int origin_j = 0;

  /* Wait for keyboard input */
  while (1) {
    curses_draw_img_with_origin(img, palette, palette_len, origin_i, origin_j);
    switch (getch()) {
    case 'h':
      origin_j -= 10;
      break;
    case 'j':
      origin_i += 10;
      break;
    case 'k':
      origin_i -= 10;
      break;
    case 'l':
      origin_j += 10;
      break;
    default:
      goto outof_keyloop;
    }
  }
 outof_keyloop:

  curses_teardown();
}
