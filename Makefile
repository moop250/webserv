# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 16:51:54 by hlibine           #+#    #+#              #
#    Updated: 2025/08/28 12:53:05 by hoannguy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Genral values
NAME        = webserv

CC          = c++

# FLAGS       = -Wall -Wextra -Werror -std=c++98
DBUG	     = -fsanitize=address
FLAGS     = -Wall -Wextra -Werror -g3 -std=c++98


OBJDIR      = .obj
INCLDIR     = headers

RM          = rm -rf

GREEN       = \033[0;32m
ORANGE      = \033[38;5;208m
LBLUE       = \033[0;36m
RESET       = \033[0m
RESETNL     = \033[0m\n

SRCS        = src/main.cpp \
	      src/utils/Colors.cpp \
	      src/utils/Debug.cpp \
	      src/utils/Error.cpp \
		  src/utils/utils.cpp \
	      src/configSetUp/Config.cpp \
	      src/configSetUp/ConfigUtils.cpp \
	      src/configSetUp/TokenUtils.cpp \
	      src/configSetUp/ConfigServ.cpp \
	      src/configSetUp/ConfigLoc.cpp \
	      src/configSetUp/ConfigError.cpp \
	      src/configSetUp/ConfigErrorCheck.cpp \
	      src/configSetUp/Server.cpp \
	      src/configSetUp/Location.cpp \
	      src/configSetUp/ErrorPages.cpp \
	      src/request/Request.cpp \
		  src/request/RequestServer.cpp \
	      src/request/support_file.cpp \
	      src/request/parse_request.cpp \
	      src/request/parse_headers.cpp\
		  src/request/Connection.cpp \
	      src/response/handle_request.cpp \
	      src/response/error_response.cpp \
	      src/response/Response.cpp \
		  src/response/handle_file_request.cpp \
	      src/sockets/SocketClass.cpp \
	      src/sockets/serverInitialization.cpp \
		  src/sockets/incomingConnection.cpp

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
