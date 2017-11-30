SUBDIR	=	${SRC_DIR}/chain_operation/ \
			${SRC_DIR}/__developer_mode/ \
			${SRC_DIR}/file_operation/ \
			${SRC_DIR}/node_operation/ \
			${SRC_DIR}/user_interface/ 

source	=	$(wildcard *.c)
OBJS	=	$(source:%.c=${OBJ_DIR}/%.o)

all:${SUBDIR} ${OBJS}

${SUBDIR}:ECHO
	make -C $@

${OBJS}:${OBJ_DIR}/%.o:%.c
	${CC} ${CFLAGS1} -c $< -o $@ ${CFLAGS2}

ECHO:
	@echo ${SUBDIR}