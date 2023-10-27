ifndef SRC_MK
SRC_MK = 1

SRC := malloc.c \
	   calloc.c \
	   realloc.c \
	   free.c \
	   structure/arena.c \
	   structure/zone.c \
	   structure/allocation.c

SIMPLE_TEST_SRC := tests/simpleMain.c

LIBKM_LOCATION = libkm/
LIBKM = $(LIBKM_LOCATION)libkm.a
LIBKM_NAME = km

endif
