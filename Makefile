# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 16:51:54 by hlibine           #+#    #+#              #
#    Updated: 2025/08/12 17:58:47 by hoannguy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Genral values
NAME        = webserv

CC          = c++
FLAGS       = -Wall -Wextra -Werror -std=c++98

OBJDIR      = .obj
INCLDIR     = headers

RM          = rm -rf

GREEN       = \033[0;32m
ORANGE      = \033[38;5;208m
LBLUE       = \033[0;36m
RESET       = \033[0m
RESETNL     = \033[0m\n

SRCS        = src/main.cpp \
			src/request/Request.cpp

OBJS        = $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(ORANGE)Compiling $(NAME)$(RESET)"
	@$(CC) $(OBJS) -o $(NAME)
	@printf "\r\033[K"
	@printf "$(GREEN)$(NAME) compiled$(RESETNL)"

# Compilation générique
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "$(ORANGE)Compiling $<...$(RESET)"
	@$(CC) $(FLAGS) -I $(INCLDIR) -c $< -o $@
	@printf "\r\033[K"
	@printf "$(GREEN)$@ compiled!$(RESETNL)"

clean:
	@printf "$(ORANGE)Cleaning up...$(RESET)"
	@$(RM) $(OBJDIR)
	@printf "\r\033[K"
	@printf "$(GREEN)Clean up successful$(RESETNL)"

fclean: clean
	@printf "$(ORANGE)Full clean up...$(RESET)"
	@$(RM) $(NAME)
	@printf "\r\033[K"
	@printf "$(GREEN)Full clean up successful$(RESETNL)"

re: fclean all

.PHONY: all clean fclean re
