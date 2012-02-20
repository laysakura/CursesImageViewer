from ctypes import *
import curses
import sys
import re

class civ_RGB(Structure):
    _fields_ = [
        ("r", c_ubyte),
        ("g", c_ubyte),
        ("b", c_ubyte),
        ("color_number", c_int),
        ]


def civ_RGB_of(curses_color_number):
    """
    @parames
    color_number: ex. curses.COLOR_BLUE

    @returns
    civ_RGB instance

    @notes
    curses.initscr and curses.start_color must be called before.
    curses.endwin must be called later.
    """
    color_tup = curses.color_content(curses_color_number)
    return civ_RGB(r=color_tup[0] * 255/1000,
                   g=color_tup[1] * 255/1000,
                   b=color_tup[2] * 255/1000,
                   color_number=curses_color_number)

def get_terminal_palette():
    try:
        curses.initscr()
        assert(curses.has_colors())
        assert(curses.start_color() != curses.ERR)

        PALETTE = civ_RGB * curses.COLORS

        # ** NOTE **
        # Indices and color_number MUST be the same.
        ret = PALETTE(
            civ_RGB_of(curses.COLOR_BLACK),   # 0
            civ_RGB_of(curses.COLOR_RED),     # 1
            civ_RGB_of(curses.COLOR_GREEN),   # 2
            civ_RGB_of(curses.COLOR_YELLOW),  # 3
            civ_RGB_of(curses.COLOR_BLUE),    # 4
            civ_RGB_of(curses.COLOR_MAGENTA), # 5
            civ_RGB_of(curses.COLOR_CYAN),    # 6
            civ_RGB_of(curses.COLOR_WHITE),   # 7
            )

        curses.endwin()
        return ret
    except Exception, ex:
        if not curses.isendwin():
            curses.endwin()
        print ex


def parse_args():
    if len(sys.argv) != 2:
        sys.stderr.write("ARG: image_path\n")
        exit(1)
    return sys.argv[1]

def get_out_img_path(img_path):
    print(img_path)
    return re.sub(r"\.([^/.]+)", r"_termcolor.\1", img_path)

def main():
    img_path = parse_args()
    out_img_path = get_out_img_path(img_path)

    libciv = cdll.LoadLibrary("./libciv-core.so")

    palette = get_terminal_palette()

    libciv.civ_color_subtraction_file2file.restype = c_int
    libciv.civ_color_subtraction_file2file.argtypes = (c_char_p, c_char_p, POINTER(civ_RGB), c_int)
    libciv.civ_color_subtraction_file2file(img_path, out_img_path, palette, 8)

if __name__ == '__main__':
    main()
