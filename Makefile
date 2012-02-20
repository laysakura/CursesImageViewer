TARGETS += curses_image_viewer

.PHONY: all
all: $(TARGETS)
	cd src ; $(MAKE) $@

.PHONY: clean
clean:
	rm $(TARGETS)
	cd src ; $(MAKE) $@

curses_image_viewer:
	cd src ; $(MAKE) $@ && \
	mv $@ ../$@
