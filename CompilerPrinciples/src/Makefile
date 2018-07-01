.PHONY: clean strip install
CC= g++
LD = g++
LEX = flex
YACC = bison

COMPILER = decafcc
DEPENDES = defs.asm dcc
PRODUCTS = $(COMPILER)
default: $(PRODUCTS)

SRCS = ast.cpp ast_decl.cpp ast_expr.cpp ast_stmt.cpp ast_type.cpp scope.cpp \
	codegen.cpp tac.cpp mips.cpp errors.cpp utility.cpp flow.cpp main.cpp printer.cpp
OBJS = y.tab.o lex.yy.o $(patsubst %.cpp, %.o, $(filter %.cpp,$(SRCS))) $(patsubst %.c, %.o, $(filter %.c, $(SRCS)))
JUNK =  *.o lex.yy.c dpp.yy.c y.tab.c y.tab.h *.core core $(COMPILER).purify purify.log

CFLAGS = -g  -Wall -Wno-unused -Wno-sign-compare -Wno-deprecated
LEXFLAGS = -d
YACCFLAGS = -dvty
LIBS = -lc -lm -lfl

.yy.o: $*.yy.c
	$(CC) $(CFLAGS) -c -o $@ $*.cpp

lex.yy.c: scanner.l  parser.y y.tab.h
	$(LEX) $(LEXFLAGS) scanner.l

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c -o y.tab.o y.tab.c

y.tab.h y.tab.c: parser.y
	$(YACC) $(YACCFLAGS) parser.y
.cpp.o: $*.cpp
	$(CC) $(CFLAGS) -c -o $@ $*.cpp

$(COMPILER) :  $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

strip : $(PRODUCTS)
	strip $(PRODUCTS)
	rm -rf $(JUNK)

clean:
	rm -f $(JUNK) y.output $(PRODUCTS)

install:
	mkdir -p build
	cp $(DEPENDES) $(COMPILER) build/

