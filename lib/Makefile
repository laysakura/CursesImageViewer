CC = gcc
CFLAGS = -Wall -O3
CFLAGS_DEBUG = -Wall -g -O3
LDFLAGS = -lcv -lml -lhighgui
INCLUDES =
SHARED_LIB_FLAGS = -shared -fPIC
OBJ_FLAGS = -fPIC

SRCS = $(wildcard *.c)
OBJS = $(subst .c,.o,$(SRCS))
DEPENDS = $(subst .c,.d,$(SRCS))

# UNROLL_FLAGS = -funroll-loops

TARGETS += libciv-core.so


.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS) $(DEBUG_TARGETS) $(TEST_TARGETS) *.o *.d *.s *.gch *_flymake* core *~

# Targets
libciv-core.so: civ_python_API.o civ_color_subtraction.o
	$(CC) $(CFLAGS) $(SHARED_LIB_FLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)


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
