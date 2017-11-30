SUBDIR	=	${SRC_DIR}/assembler ${SRC_DIR}/simulator

OBJS	=	${patsubst %.c,${OBJ_DIR}/%.o,${wildcard *.c}}

all:${SUBDIR} ${OBJS}

${SUBDIR}:ECHO
	make -C $@

${OBJS}:${OBJ_DIR}/%.o:%.c
	$(CC) -c $^ -o $@

ECHO:
	@echo " "
