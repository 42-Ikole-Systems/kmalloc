
NAME = generate_metadata
ODIR = .obj
SDIR = src
SRC := generate_metadata.c
OBJ := $(addprefix $(ODIR)/, $(SRC:.c=.o))
GENERATED_DIR = generated_includes/pre_preprocessor
GENERATED_FILENAME = $(GENERATED_DIR)/generated_kmalloc_metadata.h
CFLAGS = -Wall -Wextra -Werror
PRE_PROCESSOR_DEFINITIONS = -D KM_GENERATED_FILENAME=\"$(GENERATED_FILENAME)\"

# libraries
LIBDIR = ../libs
LIBKM_LOCATION = ../libkm/
LIBKM = $(LIBKM_LOCATION)libkm.a
LIBKM_NAME = km

# Compilation 
all: $(NAME)

$(LIBKM):
	@echo building libkm
	@$(MAKE) -C $(LIBKM_LOCATION)

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	@echo Compiling pre-preprocessor source...
	@$(CC) -c -o $@ $< -I$(LIBKM_LOCATION)/includes $(CFLAGS) $(PRE_PROCESSOR_DEFINITIONS)

$(NAME): $(LIBKM) $(OBJ)
	@echo Building metadata generator...
	@$(CC) -o $(NAME) $(OBJ) -L$(LIBDIR) -l$(LIBKM_NAME)

run: $(NAME)
	@mkdir -p $(GENERATED_DIR)
	@echo Generating $(GENERATED_FILENAME)
	@./$(NAME)

# Cleanup
clean:
	@echo Cleaning $(NAME)
	@rm -f $(GENERATED_FILENAME)
	@rm -f $(GENERATED_FILENAME)
	@rm -rf $(ODIR)

fclean: clean
	@echo Fcleaning $(NAME)
	@rm -rf $(NAME)

re: fclean all
