TOOL = pandoc
PWD = $(shell pwd)

.PHONY: all

all:
	$(TOOL) -s --css=${PWD}/../../common/sakura/sakura-dark.css --from=markdown+link_attributes --to=html result.md -o result.html
