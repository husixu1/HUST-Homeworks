SUBDIR	=	${SRC_DIR}/user_interface/main_buttons/ \
			${SRC_DIR}/user_interface/treeview_all/ \
			${SRC_DIR}/user_interface/treeview_search_result/ \

source	=	$(wildcard *.c)
OBJS	=	$(source:%.c=${OBJ_DIR}/%.o)

all:${SUBDIR} ${OBJS}

${SUBDIR}:ECHO
	make -C $@

${OBJS}:${OBJ_DIR}/%.o:%.c
	${CC} ${CFLAGS1} -c $< -o $@ ${CFLAGS2}

ECHO:
	@echo ${SUBDIR}