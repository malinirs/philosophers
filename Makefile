NAME = philo
HEADER	= philo.h

SRCS = philo.c \
		utils.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all		: $(NAME)

$(NAME) : $(OBJS) $(HEADER)
	gcc $(SRCS) -o $(NAME)

%.o : %.c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@

clean   :
	rm -f *.o

fclean	:	clean
	rm -f $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re