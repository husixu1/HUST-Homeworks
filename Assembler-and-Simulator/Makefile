CC	=	gcc

SRC_DIR	=	${shell pwd}/src
OBJ_DIR	=	${shell pwd}/obj
BIN_DIR	=	${shell pwd}/bin

CPATH	=	${SRC_DIR}:${SRC_DIR}/assembler:${SRC_DIR}/simulator
SUBDIR	=	${SRC_DIR}
TARGET	=	${BIN_DIR}

export CC SRC_DIR OBJ_DIR BIN_DIR CPATH

all:${SUBDIR} ${TARGET}

${SUBDIR}:ECHO
	@mkdir -p ${OBJ_DIR}
	make -C $@

${TARGET}:ECHO
	@mkdir -p ${BIN_DIR}
	make -C $@

ECHO:
	@echo " "

clean:
	rm -rf obj/
	make -C ${TARGET} clean
