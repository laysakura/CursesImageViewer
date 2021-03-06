#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ncurses.h>
#include "civ_config.h"
#include "civ_curses.h"
#include "civ_color.h"
#include "civ_img_processing.h"


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


/* start_color() MUST be called previously */
static inline void
color_number_to_RGB(const int color_number, civ_RGB *RGB_out)
{
  short r, g, b;
  color_content(color_number, &r, &g, &b);
  RGB_out->r = r;
  RGB_out->g = g;
  RGB_out->b = b;
}

void
get_curses_palette(civ_RGB *palette_out)
{
  curses_setup();

  int color_number;
  for (color_number = 0; color_number < CIV_CURSES_PALETTE_LEN; ++color_number) {
    civ_RGB rgb;
    color_number_to_RGB(color_number, &rgb);
    palette_out[color_number].r = rgb.r;
    palette_out[color_number].g = rgb.g;
    palette_out[color_number].b = rgb.b;
  }

  curses_teardown();
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
                            const int win_height,
                            const int win_width,
                            const int origin_i,
                            const int origin_j)
{
  int img_height = img->height;
  int img_width = img->width;

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


static inline void
fix_origin_point(const int win_height,
                 const int win_width,
                 const int img_height,
                 const int img_width,
                 int *origin_i,
                 int *origin_j)
{
  if (*origin_i < 0) *origin_i = 0;
  else if (*origin_i > img_height - win_height) *origin_i = img_height - win_height;

  if (*origin_j < 0) *origin_j = 0;
  else if (*origin_j > img_width - win_width) *origin_j = img_width - win_width;

  /* Image is scaller than window */
  if (img_height < win_height) *origin_i = 0;
  if (img_width < win_width) *origin_j = 0;
}


/* Adhoc function */
static inline void
safe_release(IplImage *img_to_release,
             const IplImage *orig_img)
{
  if (img_to_release == NULL ||
      img_to_release == orig_img)
    return;

  cvReleaseImage(&img_to_release);
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


  /* NOTE that processed_img must be released */
  IplImage *processed_img = img;

  int origin_i = 0;
  int origin_j = 0;

  /* Wait for keyboard input */
  while (1) {
    /* Calc size */
    int img_height = processed_img->height;
    int img_width = processed_img->width;
    int win_height, win_width;
    getmaxyx(stdscr, win_height, win_width);
    fix_origin_point(win_height, win_width, img_height, img_width, &origin_i, &origin_j);

    /* Draw */
    curses_draw_img_with_origin(processed_img,
                                palette, palette_len,
                                win_height, win_width,
                                origin_i, origin_j);

    /* Receive keyboard input */
    switch (getch()) {

    /* Move */
    case 'h':
      origin_j -= CIV_MOV_DIFF_UNIT;
      break;
    case 'j':
      origin_i += CIV_MOV_DIFF_UNIT;
      break;
    case 'k':
      origin_i -= CIV_MOV_DIFF_UNIT;
      break;
    case 'l':
      origin_j += CIV_MOV_DIFF_UNIT;
      break;

    /* Big move */
    case 'L':
      origin_j = 0;
      break;
    case 'R':
      origin_j = img_width - win_width;
      break;
    case 'T':
      origin_i = 0;
      break;
    case 'B':
      origin_i = img_height - win_height;
      break;
    case 'C':
      origin_i = (img_height - win_height) / 2;
      origin_j = (img_width - win_width) / 2;
      break;

    /* Back to original image */
    case 'o':
      safe_release(processed_img, img);
      processed_img = img;
      break;

    /* Resize */
    case 'f':
      safe_release(processed_img, img);
      float scale = (win_height/(float)img_height > win_width/(float)img_width ?
                     win_width/(float)img_width : win_height/(float)img_height);
      if (scale > 1.0)
        break;

      processed_img = cvCreateImage(cvSize(img_width * scale, img_height * scale),
                                    img->depth, img->nChannels);
      cvResize(img, processed_img, CV_INTER_NN);
      break;

    /* Quit */
    case 'q':
      safe_release(processed_img, img);
      goto outof_keyloop;

    default:
      break;
    }
  }
 outof_keyloop:

  curses_teardown();
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

  /* Release object */
  cvReleaseImage(&img);
}
