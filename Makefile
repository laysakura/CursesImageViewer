TARGETS += curses_image_viewer_topdir

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm curses_image_viewer
	cd src ; $(MAKE) $@

curses_image_viewer_topdir: curses_image_viewer
	cd src ; $(MAKE) $^
	mv src/$^ $^
