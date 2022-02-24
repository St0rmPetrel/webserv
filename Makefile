RED		=	\033[0;31m

YELLOW	=	\033[0;33m

GREEN	=	\033[0;32m

NC		=	\033[0m

CC		=	clang++

NAME	=	webserv

CFLAGS	=	-Wall -Werror -Wextra -g -std=c++98

OBJSDIR = objs

SRCDIR = srcs

LOGGER_DIR = $(OBJSDIR)/logger

CONFIG_DIR = $(OBJSDIR)/config

SERVER_DIR = $(OBJSDIR)/server

LOGGER =    Logger

CONFIG =	Config

SERVER	=	Server

MAIN 	=	main

SRCS = $(addprefix $(SRCDIR)/, $(addsuffix .cpp, $(MAIN))) \
	   $(addprefix $(SRCDIR)/logger/, $(addsuffix .cpp, $(LOGGER))) \
	   $(addprefix $(SRCDIR)/config/, $(addsuffix .cpp, $(CONFIG))) \
	   $(addprefix $(SRCDIR)/server/, $(addsuffix .cpp, $(SERVER)))


OBJS_BUILD	=	$(patsubst $(SRCDIR)/%,$(OBJSDIR)/%,$(SRCS:.cpp=.o))

HDRS = -I $(SRCDIR)/config -I $(SRCDIR)/logger -I $(SRCDIR)/server

DIRS	=	$(OBJSDIR) $(LOGGER_DIR) $(CONFIG_DIR) $(SERVER_DIR)

RM		=	rm -f

all:	$(NAME)

$(NAME): $(OBJS_BUILD)
		@echo "$(YELLOW)MAKING webserv from objs$(NC)"
		@$(CC) $(CFLAGS) $(OBJS_BUILD) -o $(NAME)
		@echo "$(GREEN)DONE$(NC)"

$(OBJS_BUILD):	|$(DIRS)

$(OBJSDIR):
		@echo "$(YELLOW)START making objs$(NC)"
		mkdir $(OBJSDIR)

$(LOGGER_DIR): |$(OBJSDIR)
		mkdir $(LOGGER_DIR)

$(CONFIG_DIR): |$(OBJSDIR)
		mkdir $(CONFIG_DIR)

$(SERVER_DIR): |$(OBJSDIR)
		mkdir $(SERVER_DIR)

$(OBJSDIR)/%.o:	$(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(HDRS) -c $^ -o $@


clean:
		@$(RM) -r $(OBJSDIR)
		@echo "$(RED)Delete objs dir$(NC)"

fclean:	clean
		@$(RM) $(NAME)
		@echo "$(RED)Delete exec file$(NC)"

re:		fclean all

run:	all
		./$(NAME)

.PHONY:	all clean fclean re run