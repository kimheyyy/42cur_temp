NAME = push_swap

CC = gcc
CFLAGS = -Wall -Wextra -Werror
# CFLAGS = -g3 -fsanitize=address

# ifeq ($(DEBUG),true)
# 	CFLAGS += -g
# endif

AR = ar
ARFLAGS = rc

SRCS_DIR =	./srcs				\
			./srcs/command_operations	\
			./srcs/settings	\
			./srcs/sort

OBJS_DIR = ./objs
INC_DIR = ./includes

LIBFT = libft.a
LIBFT_DIR = ./libft

SRCS =	./srcs/main.c \
		./srcs/command_operations/push.c \
		./srcs/command_operations/reverse_rotate.c \
		./srcs/command_operations/swap.c \
		./srcs/command_operations/rotate.c \
		./srcs/settings/check_input.c	\
		./srcs/settings/init_stack.c\
		./srcs/sort/control_under_three.c	\
		./srcs/sort/sort_a_to_b.c \
		./srcs/sort/sort_input_five.c \
		./srcs/sort/sort_input_three_a.c \
		./srcs/sort/sort_input_three_b.c \
		./srcs/sort/sort_b_to_a.c \
		./srcs/free.c \
		./srcs/setting_max_min.c \
		./srcs/push_swap.c

OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

vpath %.c $(SRCS_DIR)

RM = rm -f

all : $(NAME)

bonus : re
	@$(MAKE) fclean -C ./checker_bonus
	@$(MAKE) -C ./checker_bonus
	@cp ./checker_bonus/checker ./checker

$(NAME) : $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFT_DIR) -lft

$(OBJS_DIR) :
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o : %.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -o $@ -I$(INC_DIR) -I$(LIBFT_DIR) -c $^

clean :
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C checker_bonus fclean
	@$(RM) -r $(OBJS_DIR)

fclean : clean
	@$(RM) $(NAME)
	@if [ -f ./checker ]; then $(RM) ./checker; fi;

re : fclean all