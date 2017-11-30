OBJ_DIR	=	$(shell pwd)/obj
BIN_DIR	=	$(shell pwd)/bin
SRC_DIR =	$(shell pwd)/src
CPATH	=	${SRC_DIR}:${SRC_DIR}/chain_operation/:${SRC_DIR}/data_structure/:${SRC_DIR}/__developer_mode/:${SRC_DIR}/file_operation/:${SRC_DIR}/node_operation/:${SRC_DIR}/user_interface/:${SRC_DIR}/user_interface/main_buttons/:${SRC_DIR}/user_interface/treeview_all/:${SRC_DIR}/user_interface/treeview_search_result/:/usr/include/gtk-3.0/gtk
SUBDIR	=	${SRC_DIR}

CC		=	gcc
CFLAGS1	=	`pkg-config --cflags gtk+-3.0` -Wl,--export-dynamic
CFLAGS2	=	`pkg-config --libs gtk+-3.0` `pkg-config --cflags --libs gmodule-2.0` 

export CC CFLAGS1 CFLAGS2 OBJ_DIR INC_DIR SRC_DIR CPATH

all:$(SUBDIR) ${BIN_DIR}

$(SUBDIR):ECHO
	@mkdir -p obj
	make -C $@

${BIN_DIR}:ECHO
	make -C $@

clean:
	rm -rf ${OBJ_DIR}/*
	rm -rf ${BIN_DIR}/NDDIMS
	make -C ${BIN_DIR} clean
ECHO:
	@echo ${SUBDIR}
