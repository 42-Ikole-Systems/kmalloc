ifndef SETTINGS_MK
SETTINGS_MK = 1

NAME	= libkmalloc.a
LIBNAME = kmalloc
LIBDIR	= libs

CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= $(addprefix -I, $(IDIR))

SDIR	= src
ODIR	= .obj
IDIR	= includes $(LIBKM_LOCATION)/includes $(PRE_PREPROCESSOR_INCLUDE)

DEPENDENCIES := $(patsubst %.c,.obj/%.d,$(SRC))

TEST_DIR = tests
UNIT_DIR = $(TEST_DIR)/unit_tests

SIMPLE_TEST_NAME	= simple_test
UNIT_TESTS			= $(wildcard $(UNIT_DIR)/*.c)
UNIT_BIN			= $(patsubst $(UNIT_DIR)/%.c, $(UNIT_DIR)/bin/%, $(UNIT_TESTS))
UNIT_TEST_INCLUDES	= -Isrc 

ifndef DEBUG
	CFLAGS += -O3
else
	CFLAGS += -Og -g -D DEBUG
endif

ifdef LEAKS
	CFLAGS += -D LEAKS
endif

ifdef FSANITIZE
	CFLAGS += -D FSANITIZE
	CFLAGS += -g -fsanitize=address
endif

include ./make_settings/src.mk
OBJ := $(addprefix $(ODIR)/, $(SRC:.c=.o))
UNIT_OBJ := $(filter-out $(ODIR)/main.o,$(OBJ))

endif
