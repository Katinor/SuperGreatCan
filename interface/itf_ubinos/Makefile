#CONFIG_ARCH_COMMON = arm
#CONFIG_ARCH = armclassic
#CONFIG_BOARD = sam7xek
#CONFIG = armclassic_sam7xek
#OUTDIR = out
#ITFDIR = $(realpath ../)

SRCDIR = $(realpath src)

.PHONY : all config build clean cleanall

all: ubiconfig.h itf/type.h
	@echo "\n[$$(pwd) : make "$@"]"

config: ubiconfig.h itf/type.h
	@echo "\n[$$(pwd) : make "$@"]"

build: ubiconfig.h itf/type.h
	@echo "\n[$$(pwd) : make "$@"]"

clean: ubiconfig.h itf/type.h
	@echo "\n[$$(pwd) : make "$@"]"

cleanall: clean
	@echo "\n[$$(pwd) : make "$@"]"
	rm -f ubiconfig.h
	rm -f itf/type.h

rebuild: clean all
	@echo "\n[$$(pwd) : make "$@"]"

rebuildall:
	@echo "\n[$$(pwd) : make "$@"]"
	make cleanall
	make all

ubiconfig.h:
	cp -f cfg/$(CONFIG)/ubiconfig.h ubiconfig.h
itf/type.h:
	cp -f cfg/$(CONFIG)/itf/type.h itf/type.h
