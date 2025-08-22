# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hlibine <hlibine@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 16:51:54 by hlibine           #+#    #+#              #
#    Updated: 2025/05/29 15:49:26 by hlibine          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Genral values
NAME        = webserv

CC          = c++
DBUG	     = -g#-fsanitize=address
FLAGS       = -Wall -Wextra -Werror -g3 -std=c++98

OBJDIR      = .obj
INCLDIR     = headers

RM          = rm -rf

GREEN       = \033[0;32m
ORANGE      = \033[38;5;208m
LBLUE       = \033[0;36m
RESET       = \033[0m
RESETNL     = \033[0m\n

SRCS        = src/main.cpp \
			src/utils/Colors.cpp src/utils/Debug.cpp src/utils/Error.cpp \
	      	src/configSetUp/Config.cpp \
            src/configSetUp/ConfigUtils.cpp \
			src/configSetUp/TokenUtils.cpp \
			src/configSetUp/ConfigServ.cpp \
			src/configSetUp/ConfigLoc.cpp \
			src/configSetUp/ConfigError.cpp \
			src/configSetUp/ConfigErrorCheck.cpp \
            src/sockets/SocketClass.cpp \
			src/sockets/serverInitialization.cpp 

OBJS        = $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(ORANGE)Compiling $(NAME)$(RESET)"
	@$(CC) $(OBJS) $(DBUG) -o $(NAME)
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
