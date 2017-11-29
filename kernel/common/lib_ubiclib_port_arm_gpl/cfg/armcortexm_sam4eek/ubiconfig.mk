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
#CFLAG_ARCH                      = -mcpu=cortex-m4 -mlittle-endian -msoft-float
CFLAG_ARCH                      = -mcpu=cortex-m4 -mlittle-endian
CFLAG_ARCH						+= -D__SAM4E16E__
CFLAG_ARCH						+= -DBOARD=SAM4E_EK
CFLAG_ARCH						+= -std=c99
CFLAG_OPSTATE                   = -mthumb
CFALG_OPTIMIZATION              = -O0
CFALG_DEBUG                     = -g3
CFLAG_INCLUDE                   = -I$(ITFDIR)
CFALG_MISC                      = -fomit-frame-pointer -Werror -Wall -fmessage-length=0 -c

ASFLAG_ARCH                      = -mcpu=cortex-m4 -mlittle-endian
ASFLAG_ARCH						+= -D__SAM4E16E__
ASFLAG_ARCH						+= -DBOARD=SAM4E_EK
ASFLAG_ARCH						+= -std=c99

CFLAG_SFPU						= -DIN_GCC -DCROSS_DIRECTORY_STRUCTURE -DIN_LIBGCC2 -D__GCC_FLOAT_NOT_NEEDED -DHAVE_CC_TLS
CFLAG_SFPU						+= -W -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -fno-inline

CFLAGS                          = $(CFLAG_ARCH) $(CFALG_OPTIMIZATION) $(CFALG_DEBUG) $(CFLAG_INCLUDE) $(CFALG_MISC) $(CFLAG_SFPU)
CXXFLAGS                        = $(CFLAGS)
ASFLAGS                         = $(CFLAGS)
ASFLAGS                         += -xassembler-with-cpp

ARFLAGS                         = rcs
LDFLAGS                         = -mcpu=cortex-m4 -nostartfiles -nostdlib

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

TARGET                          = lib_ubiclib_port_arm_gpl

CSRCS_default				   += ubiclib_port.c
CSRCS_default				   += div0.c

ASRCS_default				   += addsubdf3.S
ASRCS_default				   += addsubsf3.S
ASRCS_default				   += ashldi3.S
ASRCS_default				   += ashrdi3.S
ASRCS_default				   += bb_init_func.S
ASRCS_default				   += call_via_rX.S
ASRCS_default				   += cmpdf2.S
ASRCS_default				   += cmpsf2.S
ASRCS_default				   += divsi3.S
ASRCS_default				   += fixdfsi.S
ASRCS_default				   += fixsfsi.S
ASRCS_default				   += fixunsdfsi.S
ASRCS_default				   += fixunssfsi.S
ASRCS_default				   += floatdidf.S
ASRCS_default				   += floatdisf.S
ASRCS_default				   += floatundidf.S
ASRCS_default				   += floatundisf.S
ASRCS_default				   += interwork_call_via_rX.S
ASRCS_default				   += lshrdi3.S
ASRCS_default				   += modsi3.S
ASRCS_default				   += muldivdf3.S
ASRCS_default				   += muldivsf3.S
ASRCS_default				   += negdf2.S
ASRCS_default				   += negsf2.S
ASRCS_default				   += truncdfsf2.S
ASRCS_default				   += udivsi3.S
ASRCS_default				   += umodsi3.S
ASRCS_default				   += unorddf2.S
ASRCS_default				   += unordsf2.S

COBJS_default                  += $(patsubst %.c,%.o, $(CSRCS_default))
$(COBJS_default) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_default) $(CFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_default)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_default))

CSRCS_nosocheck                += uitoa_nosocheck.c
COBJS_nosocheck                += $(patsubst %.c,%.o, $(CSRCS_nosocheck))
$(COBJS_nosocheck) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_nosocheck) $(CFLAGS_nosocheck) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_nosocheck)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_nosocheck))

ASOBJS_default				   += $(patsubst %.S,%.o, $(ASRCS_default))
$(ASOBJS_default) : %.o: $(SRCDIR)/%.S
	@$(MKDIR) $(dir $@)
	$(AS_default) $(ASFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS						   +=$(ASOBJS_default)
DEPS						   +=$(patsubst %.o,%.d, $(ASOBJS_default))

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
