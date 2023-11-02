SETTINGS_DIR := make_settings

include $(SETTINGS_DIR)/src.mk
include $(SETTINGS_DIR)/settings.mk
include $(SETTINGS_DIR)/colors.mk

all: $(LIBDIR)/$(NAME)

# Compilation

$(LIBDIR)/$(NAME): $(PRE_PREPROCESSOR_DEPENDENCY) $(LIBDIR) $(LIBKM) $(OBJ)
	@echo "$(COLOR_GREEN)Creating $(NAME) library...$(COLOR_RESET)"
	@ar rcs $(LIBDIR)/$(NAME) $(OBJ)

-include $(DEPENDENCIES)

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	@echo "$(COLOR_LBLUE)Compiling...	$(COLOR_BLUE)$<$(COLOR_RESET)"
	@$(CC) -c -o $@ $< $(CFLAGS) -MMD -MP $(IFLAGS)

$(LIBKM): $(LIBDIR)
	@$(MAKE) -C $(LIBKM_LOCATION)
	@cp $(LIBKM) $(LIBDIR)/

$(LIBDIR):
	@echo "creating build folder."
	@mkdir $(LIBDIR)

$(PRE_PREPROCESSOR_DEPENDENCY):
	@$(MAKE) run -C $(PRE_PREPROCESSOR_LOCATION)

# Clean up

clean:
	@echo "$(COLOR_YELLOW)cleaning $(NAME)... $(COLOR_RESET)"
	@$(MAKE) clean -C $(LIBKM_LOCATION)
	@$(MAKE) clean -C $(PRE_PREPROCESSOR_LOCATION)
	@printf "$(COLOR_RED)"
	$(RM) -r $(ODIR)
	@printf "$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_YELLOW)force cleaning $(NAME)... $(COLOR_RESET)"
	@$(MAKE) fclean -C $(LIBKM_LOCATION)
	@$(MAKE) fclean -C $(PRE_PREPROCESSOR_LOCATION)
	@printf "$(COLOR_RED)"
	$(RM) $(NAME) $(SIMPLE_TEST_NAME)
	$(RM) -rf $(UNIT_BIN) $(LIBDIR)
	@printf "$(COLOR_RESET)"

re: fclean
	@$(MAKE) all

# Unit tests

$(UNIT_DIR)/bin/%: $(UNIT_DIR)/%.c
	@mkdir -p $(UNIT_DIR)/bin
	@echo "$(COLOR_LBLUE)Compiling tests... $(COLOR_BLUE)$<$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) $(UNIT_TEST_INCLUDES) $< -o $@ -lcriterion -L$(LIBDIR) -l$(LIBNAME) -l$(LIBKM_NAME)

unit_test_build: $(LIBDIR)/$(NAME) $(UNIT_DIR) $(UNIT_BIN)

unit_test: unit_test_build
	@sh $(UNIT_DIR)/run_tests.sh

test: $(LIBDIR)/$(NAME)
	@$(CC) $(SIMPLE_TEST_SRC) $(CFLAGS) $(IFLAGS) $(UNIT_TEST_INCLUDES) -L$(LIBDIR) -l$(LIBNAME) -l$(LIBKM_NAME) -o $(SIMPLE_TEST_NAME)

# Debugging
debug: fclean
	@echo "$(COLOR_YELLOW)Building $(NAME) debug... $(COLOR_RESET)"
	@$(MAKE) debug -C $(LIBKM_LOCATION)
	@$(MAKE) DEBUG=1

leaks: fclean
	@echo "$(COLOR_YELLOW)Building $(NAME) leaks... $(COLOR_RESET)"
	@$(MAKE) leaks -C $(LIBKM_LOCATION)
	@$(MAKE) LEAKS=1

fsanitize: fclean
	@echo "$(COLOR_YELLOW)Building $(NAME) fsanitize... $(COLOR_RESET)"
	@$(MAKE) fsanitize -C $(LIBKM_LOCATION)
	@$(MAKE) FSANITIZE=1

# Phony
.PHONY: debug fsanitize test clean fclean re
