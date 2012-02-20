CC = gcc
CFLAGS = -Wall -O3
CFLAGS_DEBUG = -Wall -g -O3
LDFLAGS = -lcv -lml -lhighgui -lncurses
INCLUDES =
OBJ_FLAGS = -fPIC

SRCS = $(wildcard *.c)
OBJS = $(subst .c,.o,$(SRCS))
DEPENDS = $(subst .c,.d,$(SRCS))


TARGETS += curses_image_viewer

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o *.d *.s *.gch *_flymake* core *~

# Targets
curses_image_viewer: civ_main.o civ_color_subtraction.o civ_curses.o
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)


.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) $(OBJ_FLAGS) $(INCLUDES) -c $< -o $@ $(LDFLAGS)

%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
-include $(DEPENDS)

.PHONY: check-syntax
check-syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only $(CHK_SOURCES)
