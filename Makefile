NAME		= ft_malcolm

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP
INCLUDES	= -Iincludes

SRCS_DIR	= srcs
OBJS_DIR	= objs

SRCS		= $(wildcard $(SRCS_DIR)/*.c)
OBJS		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

RM			= rm -f
RMDIR		= rm -rf

GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) built successfully.$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	@$(RMDIR) $(OBJS_DIR)
	@echo "$(YELLOW)Object files removed.$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(YELLOW)$(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
