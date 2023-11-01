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

PRE_PREPROCESSOR_LOCATION = pre_preprocessor/
PRE_PREPROCESSOR_INCLUDE = $(PRE_PREPROCESSOR_LOCATION)/generated_includes
PRE_PREPROCESSOR_DEPENDENCY = $(PRE_PREPROCESSOR_LOCATION)/pre_preprocessor/generated_kmalloc_metadata.h

endif
