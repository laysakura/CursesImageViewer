CC = gcc
CFLAGS = -Wall -O3
CFLAGS_DEBUG = -Wall -g -O3
LDFLAGS = -lcv -lhighgui -lncurses
INCLUDES =
OBJ_FLAGS = -fPIC

SRCS = $(wildcard src/*.c)
OBJS = $(subst .c,.o,$(SRCS))


TARGETS += curses_image_viewer
TARGETS_DEPENDENCY += $(OBJS)

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS) $(OBJS)

# Targets
curses_image_viewer: $(TARGETS_DEPENDENCY)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)


.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) $(OBJ_FLAGS) $(INCLUDES) -c $< -o $@ $(LDFLAGS)
