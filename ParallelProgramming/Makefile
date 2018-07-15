.PHONY: all clean

SUBDIRS = lab1 lab2 lab3 lab4 lab5 project1

all:
	for lab in ${SUBDIRS}; do \
		make -C $$lab; \
	done

clean:
	for lab in ${SUBDIRS}; do \
		make -C $$lab clean; \
	done
