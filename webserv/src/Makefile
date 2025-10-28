# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hoannguy <hoannguy@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 16:51:54 by hlibine           #+#    #+#              #
#    Updated: 2025/09/07 12:30:22 by hoannguy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Genral values
NAME        = webserv

CC          = c++
FLAGS       = -Wall -Wextra -Werror -std=c++98
FLAGS =

OBJDIR      = .obj
INCLDIR     = ../headers

RM          = rm -rf

GREEN       = \033[0;32m
ORANGE      = \033[38;5;208m
LBLUE       = \033[0;36m
RESET       = \033[0m
RESETNL     = \033[0m\n

SRCS        = tdd.cpp catch_amalgamated.cpp \
		  utils/Colors.cpp \
	      utils/Debug.cpp \
	      utils/Error.cpp \
		  utils/utils.cpp \
	      configSetUp/Config.cpp \
	      configSetUp/ConfigUtils.cpp \
	      configSetUp/TokenUtils.cpp \
	      configSetUp/ConfigServ.cpp \
	      configSetUp/ConfigLoc.cpp \
	      configSetUp/ConfigError.cpp \
	      configSetUp/ConfigErrorCheck.cpp \
	      configSetUp/Server.cpp \
	      configSetUp/Location.cpp \
	      configSetUp/ErrorPages.cpp \
	      request/Request.cpp \
	      request/support_file.cpp \
	      request/parse_request.cpp \
	      request/parse_headers.cpp\
		  request/RequestServer.cpp \
		  request/Connection.cpp \
	      response/handle_request.cpp \
	      response/error_response.cpp \
	      response/Response.cpp \
		  response/handle_file_request.cpp \
		  response/handle_directory_request.cpp \
		  response/handle_CGI_request.cpp \
	      sockets/SocketClass.cpp \
	      sockets/serverInitialization.cpp \
		  sockets/incomingConnection.cpp \
		  sockets/GenFD.cpp

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
