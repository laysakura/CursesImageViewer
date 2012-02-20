#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "civ_config.h"
#include "civ_color.h"
#include "civ_curses.h"

void
print_supported_format()
{
  fprintf(stderr, "Supported format:\n");
  fprintf(stderr, "\tBMP,DIB\n");
  fprintf(stderr, "\tJPEG,JPG,JPE\n");
  fprintf(stderr, "\tPNG\n");
  fprintf(stderr, "\tPBM,PGM,PPM\n");
  fprintf(stderr, "\tSR,RAS\n");
  fprintf(stderr, "\tTIFF,TIF\n");
  fprintf(stderr, "\tEXR\n");
  fprintf(stderr, "\tJP2\n");
}

int
main(int argc, char **argv)
{
  /* Parse arguments and options */
  if (argc != 2) {
    fprintf(stderr, "ARG: Path_to_image_file\n");
    print_supported_format();
    exit(1);
  }
  char *img_path = argv[1];

  /* TODO: Support `colorful' terminal */
  assert(CIV_CURSES_PALETTE_LEN == 8);
  civ_RGB palette[CIV_CURSES_PALETTE_LEN];
  get_curses_palette(palette);

  civ_curses_draw_img(img_path, palette, CIV_CURSES_PALETTE_LEN);

  return 0;
}
