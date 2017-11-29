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
#CFLAG_ARCH                      = -mcpu=cortex-m4 -mlittle-endian -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -DARM_MATH_CM4
CFLAG_ARCH                      = -mcpu=cortex-m4 -mlittle-endian -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -DARM_MATH_CM4 -D__VFP_FP__ -D__FPU_PRESENT=1
CFLAG_ARCH						+= -D__SAM4E16E__
CFLAG_ARCH						+= -DBOARD=SAM4E_EK
CFLAG_ARCH						+= -std=c99
CFLAG_OPSTATE                   = -mthumb
CFALG_OPTIMIZATION              = -O0
CFALG_DEBUG                     = -g3
CFLAG_INCLUDE                   = -I$(ITFDIR)
CFALG_MISC                      = -fomit-frame-pointer -Werror -Wall -fmessage-length=0 -c

CFLAGS                          = $(CFLAG_ARCH) $(CFALG_OPTIMIZATION) $(CFALG_DEBUG) $(CFLAG_INCLUDE) $(CFALG_MISC)
CXXFLAGS                        = $(CFLAGS)
ASFLAGS                         = $(CFLAGS)

ARFLAGS                         = rcs
LDFLAGS                         = -mcpu=cortex-m4 -nostartfiles -nostdlib -mfloat-abi=softfp -mfpu=fpv4-sp-d16

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

CC_assemble                     = $(CC)
CFLAGS_assemble                 = $(CFLAG_ARCH) $(CFALG_OPTIMIZATION) -fomit-frame-pointer -Werror -Wall -fmessage-length=0 $(CFLAG_OPSTATE)

################

OBJS                            = 
DEPS                            = 
LIBS                            = 

################

TARGET                          = lib_ubiclib_arm

CSRCS_default                  += ubiclib_arch.c moddiv.c uidivmod.c
COBJS_default                  += $(patsubst %.c,%.o, $(CSRCS_default))
$(COBJS_default) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_default) $(CFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_default)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_default))

ASSRCS_default                 += 
ASOBJS_default                 += $(patsubst %.S,%.o, $(ASSRCS_default))
$(ASOBJS_default) : %.o: $(SRCDIR)/%.S
	$(AS_default) $(ASFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(ASOBJS_default)
DEPS                           += $(patsubst %.o,%.d, $(ASOBJS_default))

CSRCS_assemble                 += 
COBJS_assemble                 += $(patsubst %.c,%.s, $(CSRCS_assemble))
$(COBJS_assemble) : %.s: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_assemble) $(CFLAGS_assemble) -MD -MP -MF"$(@:%.s=%.d)" -S "$<"	
OBJS                           += $(COBJS_assemble)
DEPS                           += $(patsubst %.s,%.d, $(COBJS_assemble))

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
