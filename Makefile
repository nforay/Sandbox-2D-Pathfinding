NAME		= sandbox
CC			= clang++
CFLAGS		= #-Wall -Wextra -Werror
SRC_PATH	= srcs/
OBJ_PATH	= objs/
INC_PATH	= incs/
INC			= -I $(INC_PATH)

ifeq ($(DB),1)
	CFLAGS	+= -g
else ifeq ($(DB),2)
	CFLAGS	+= -g3 -fsanitize=address
else ifeq ($(DB),3)
	CFLAGS	=
endif

DEF			= \033[0m
RED			= \033[31;1m
GRN			= \033[32;1m
YEL			= \033[33;1m

SRCS		= main.cpp TileMap.cpp
OBJS		= $(SRCS:.cpp=.o)

OBJ			= $(addprefix $(OBJ_PATH), $(OBJS))

all: $(NAME)

$(NAME):		$(OBJ)
				@echo "$(RED)[$(NAME)] : $(DEF)Linking..."
				$(CC) $(CFLAGS) $(INC) $(OBJ) -lsfml-graphics -lsfml-window -lsfml-system -o $(NAME)
				@echo "$(RED)[$(NAME)] : $(DEF)Compilation \t\t$(GRN)[OK]$(DEF)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
				@mkdir $(OBJ_PATH) 2> /dev/null || true
				@echo -n "$(RED)[$(NAME)] :$(DEF) Compiling $< \t"
				@$(CC) $(CFLAGS) $(INC) -o $@ -c $< && echo "$(GRN)[OK]$(DEF)" || echo "$(RED)[KO]$(DEF)"

clean:
				@echo "$(YEL)[$(NAME)] : $(DEF)Cleaning..."
				@rm -rf $(OBJ_PATH)

fclean:			clean
				@rm -rf $(NAME)

gdb:			$(NAME)
				gdb ./$<

norm:
				@~/.norminette/norminette.rb $(SRC_PATH)
				@~/.norminette/norminette.rb $(INC_PATH)

re:				fclean all

.PHONY: all re clean fclean
