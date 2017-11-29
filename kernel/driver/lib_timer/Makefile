#CONFIG_ARCH_COMMON = arm
#CONFIG_ARCH = armclassic
#CONFIG_BOARD = sam7xek
#CONFIG = armclassic_sam7xek
#OUTDIR = out
#ITFDIR = $(realpath ../../../interface)

SRCDIR = $(realpath src)

MAKE_FUNC = \
make $(1) \
-C $(OUTDIR) \
ITFDIR=$(ITFDIR) \
SRCDIR=$(SRCDIR)

.PHONY : all config build clean cleanall rebuild rebuildall

all: ubiconfig.h ubiconfig.mk $(OUTDIR)/Makefile
	@echo "\n[$$(pwd) : make "$@"]"
	$(call MAKE_FUNC,$@)

config: ubiconfig.h ubiconfig.mk $(OUTDIR)
	@echo "\n[$$(pwd) : make "$@"]"
	cp -f ubiconfig.mk $(OUTDIR)/Makefile
	$(call MAKE_FUNC,$@)

build: ubiconfig.h ubiconfig.mk $(OUTDIR)/Makefile
	@echo "\n[$$(pwd) : make "$@"]"
	$(call MAKE_FUNC,$@)

clean: ubiconfig.h ubiconfig.mk $(OUTDIR)/Makefile
	@echo "\n[$$(pwd) : make "$@"]"
	$(call MAKE_FUNC,$@)

cleanall: clean
	@echo "\n[$$(pwd) : make "$@"]"
	rm -f ubiconfig.h
	rm -f ubiconfig.mk
	rm -f $(OUTDIR)/Makefile

rebuild: clean all
	@echo "\n[$$(pwd) : make "$@"]"

rebuildall:
	@echo "\n[$$(pwd) : make "$@"]"
	make cleanall
	make all

$(OUTDIR):
	mkdir -p $(OUTDIR)
$(OUTDIR)/Makefile: $(OUTDIR) config

ubiconfig.h:
	cp -f cfg/$(CONFIG)/ubiconfig.h  ubiconfig.h
ubiconfig.mk:
	cp -f cfg/$(CONFIG)/ubiconfig.mk ubiconfig.mk
		
