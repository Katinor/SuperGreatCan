################

#ITFDIR=../../
#SRCDIR=../src

################

RM                              = rm -rf
MKDIR                           = mkdir -p
RMDIR                           = rmdir -p
CP                              = cp -rf
INSTALL_EXE                     = install -m755
UNPACK                          = tar xvfj

################

TARGET_TOOLCHAIN_TYPE           = armnoneeabi
TARGET_TOOLCHAIN_PREFIX         = arm-none-eabi-

CC                              = $(TARGET_TOOLCHAIN_PREFIX)gcc
CXX                             = $(TARGET_TOOLCHAIN_PREFIX)g++
AS                              = $(TARGET_TOOLCHAIN_PREFIX)gcc

AR                              = $(TARGET_TOOLCHAIN_PREFIX)ar
LD                              = $(TARGET_TOOLCHAIN_PREFIX)g++

#CFLAG_ARCH                      = -mcpu=arm7tdmi -mthumb-interwork -mlittle-endian -msoft-float -D__SOCHECK__ -msocheck -D__SUCHECK__ -msucheck 
CFLAG_ARCH                      = -mcpu=cortex-m4 -mlittle-endian -msoft-float
CFLAG_OPSTATE                   = -mthumb
CFALG_OPTIMIZATION              = -O0
CFALG_DEBUG                     = -g3
CFLAG_INCLUDE                   = -I$(ITFDIR)
CFALG_MISC                      = -fomit-frame-pointer -Werror -Wall -fmessage-length=0 -c

CFLAGS                          = $(CFLAG_ARCH) $(CFALG_OPTIMIZATION) $(CFALG_DEBUG) $(CFLAG_INCLUDE) $(CFALG_MISC)
CXXFLAGS                        = $(CFLAGS)
ASFLAGS                         = $(CFLAGS)

ARFLAGS                         = rcs
LDFLAGS                         = 

CC_default                      = $(CC)
CFLAGS_default                  = $(CFLAGS) $(CFLAG_OPSTATE)

CXX_default                     = $(CXX)
CXXFLAGS_default                = $(CXXFLAGS) $(CFLAG_OPSTATE)

AS_default                      = $(AS)
ASFLAGS_default                 = $(ASFLAGS) $(CFLAG_OPSTATE) -D__ASSEMBLY__

CC__arm                         = $(CC)
CFLAGS__arm                     = $(CFLAGS) -mthumb

AS__arm                         = $(AS)
ASFLAGS__arm                    = $(ASFLAGS) -mthumb -D__ASSEMBLY__

CC__thumb                       = $(CC)
CFLAGS__thumb                   = $(CFLAGS) -mthumb

AS__thumb                       = $(AS)
ASFLAGS__thumb                  = $(ASFLAGS) -mthumb -D__ASSEMBLY__

CC_nosocheck                    = $(CC)
#CFLAGS_nosocheck                = $(CFLAGS) $(CFLAG_OPSTATE) -D__NOSOCHECK__ -mnosocheck -D__NOSUCHECK__ -mnosucheck
CFLAGS_nosocheck                = $(CFLAGS) $(CFLAG_OPSTATE)

CC_nosocheck__arm               = $(CC)
#CFLAGS_nosocheck__arm           = $(CFLAGS) -mthumb -D__NOSOCHECK__ -mnosocheck -D__NOSUCHECK__ -mnosucheck
CFLAGS_nosocheck__arm           = $(CFLAGS) -mthumb

CC_nosocheck__thumb             = $(CC)
#CFLAGS_nosocheck__thumb         = $(CFLAGS) -mthumb -D__NOSOCHECK__ -mnosocheck -D__NOSUCHECK__ -mnosucheck
CFLAGS_nosocheck__thumb         = $(CFLAGS) -mthumb

################

OBJS                            = 
DEPS                            = 
LIBS                            = 

################

TARGET                          = lib_ubik_armcortexm

CSRCS_default                  += ubik_arch.c
COBJS_default                  += $(patsubst %.c,%.o, $(CSRCS_default))
$(COBJS_default) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_default) $(CFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_default)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_default))

CSRCS__arm                     += ubik_arch__arm.c
COBJS__arm                     += $(patsubst %.c,%.o, $(CSRCS__arm))
$(COBJS__arm) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC__arm) $(CFLAGS__arm) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS__arm)
DEPS                           += $(patsubst %.o,%.d, $(COBJS__arm))

CSRCS_nosocheck                  += ubik_arch_nosocheck.c
COBJS_nosocheck                  += $(patsubst %.c,%.o, $(CSRCS_nosocheck))
$(COBJS_nosocheck) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_nosocheck) $(CFLAGS_nosocheck) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_nosocheck)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_nosocheck))

CSRCS_nosocheck__arm                  += ubik_arch_nosocheck__arm.c
COBJS_nosocheck__arm                  += $(patsubst %.c,%.o, $(CSRCS_nosocheck__arm))
$(COBJS_nosocheck__arm) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_nosocheck__arm) $(CFLAGS_nosocheck__arm) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_nosocheck__arm)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_nosocheck__arm))

################

.PHONY : all config compile link clean

################

all: config compile link
	@echo "\n[$$(pwd) : make "$@"]"

config:
	@echo "\n[$$(pwd) : make "$@"]"

build: compile link
	@echo "\n[$$(pwd) : make "$@"]"

compile: $(OBJS)
	@echo "\n[$$(pwd) : make "$@"]"

link: $(OBJS)
	@echo "\n[$$(pwd) : make "$@"]"
	$(AR) $(ARFLAGS) lib$(TARGET_TOOLCHAIN_TYPE)__$(TARGET).a $(OBJS)

clean:
	@echo "\n[$$(pwd) : make "$@"]"
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) lib$(TARGET_TOOLCHAIN_TYPE)__$(TARGET).a

################

-include $(DEPS)

################
