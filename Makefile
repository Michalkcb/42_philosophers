# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/05 19:55:56 by mbany             #+#    #+#              #
#    Updated: 2024/12/07 14:55:54 by mbany            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the final executable
NAME = philo

# Source files
SRC_DIR = ./
SRC = main.c utils.c init.c threads.c monitor.c actions.c
SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))


# Object files directory and list
OBJ_DIR = obj/
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -pthread

# Header files
HEADER = $(SRC_DIR)philo.h

.PHONY: all clean fclean re debug

# Kolory dla lepszej czytelności
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

# Default rule
all: $(NAME)

# Kompilacja programu
$(NAME): $(OBJS) $(HEADER)
	@echo "$(GREEN)Linking executable $(NAME)...$(RESET)"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS)

# Kompilacja plików .c na .o z paskiem postępu
$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER)
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "[ $(shell find $(OBJ_DIR) -type f | wc -l)/$(words $(SRC)) files compiled ]"

# Czyszczenie plików obiektowych
clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

# Czyszczenie wszystkiego
fclean: clean
	@echo "$(YELLOW)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)

# Rebuild the project
re: fclean all

# Debug mode with additional flags
debug: CFLAGS += -g
debug: re
