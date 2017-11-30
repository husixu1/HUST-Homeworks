OBJS	=	${patsubst %.c,${OBJ_DIR}/%.o,${wildcard *.c}}

all:${OBJS}

${OBJS}:${OBJ_DIR}/%.o:%.c
	$(CC) -c $^ -o $@
