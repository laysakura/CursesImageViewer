#include <ncurses.h>
#include "civ_curses.h"
#include "civ_color.h"


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

#include <unistd.h>

void
curses_draw_img(const IplImage *img,
                const civ_RGB *palette,
                const int palette_len)
{
  curses_setup();

  int i, j;
  for (i = 0; i < img->height; ++i) {
    for (j = 0; j < img->width; ++j) {
      CvScalar pixel_color = cvGet2D(img, i, j);

      /* Find the closest color to pixel_color from palette */
      int new_color_number;
      nearest_color_number_from_palette(&pixel_color,
                                        palette,
                                        palette_len,
                                        &new_color_number);

      /* Draw pixel */
      curses_draw_pixel(i, j, new_color_number);
    }
  }
  refresh();
  int key = getch();

  curses_teardown();
}
