source	=	$(wildcard *.c)
OBJS	=	$(source:%.c=${OBJ_DIR}/%.o)

all:${OBJS}
	@echo done

${OBJS}:${OBJ_DIR}/%.o:%.c
	${CC} ${CFLAGS1} -c $< -o $@ ${CFLAGS2}
