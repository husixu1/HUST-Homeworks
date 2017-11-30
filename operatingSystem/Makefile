# Makefile written by Sixu Hu 2017-11-16

# each directory stands for a project
PROJECTS = $(shell find . -type d -iname 'experiment*' -printf ' %f')

.PHONY: all clean

# recursively make all projects
all: ${PROJECTS}
	@for target in ${PROJECTS}; do \
		make -C $$target; \
	done

clean: ${PROJECTS}
	@for target in ${PROJECTS}; do \
	    make -C $$target clean; \
	done
