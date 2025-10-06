.DEFAULT_GOAL := all

NAME = calculator

SOURCE = main.c tinyexpr.c

OBJ_DIR = obj

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCE:.c=.o))

GCC = gcc
FLAGS = -Wextra -Wall -Werror
# FLAGS =
RM = rm -f

all : $(NAME)

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(OBJ_DIR)
	@$(GCC) $(FLAGS) -c $< -o $@


$(NAME):	$(OBJECTS)
	@$(GCC) $(FLAGS) $(OBJECTS) -o $(NAME) -lm
	@echo "Built $(NAME)"

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "Deleted object files"

fclean:		clean
	@$(RM) $(NAME)
	@echo "Deleted $(NAME)"

re: fclean all

.PHONY: all clean fclean re