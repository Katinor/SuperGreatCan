#bsp_armcortexm_sam4eek
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
CFLAG_OPSTATE                   = -mthumb
CFALG_OPTIMIZATION              = -O0
CFALG_DEBUG                     = -g3
CFLAG_SAM						+= -D__SAM4E16E__
CFLAG_SAM						+= -DBOARD=SAM4E_EK

INCLUDE_MCU_CORE				= -I$(ITFDIR)/../kernel/porting/lib_sam4e/thirdparty/CMSIS/Include
#INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../CORTEXM_CMSIS/Include
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include/component
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include/instance
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/fpu
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/header_files
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/preprocessor
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/common/utils
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/boards/sam4e_ek/board_config
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/common/boards
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/common/services/ioport
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/common/services/clock
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/common/services/clock/sam4e/module_config
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/boards/sam4e_ek
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/source/templates

SAMLIB_PATH						= $(ITFDIR)/../kernel/porting/lib_sam4e
INCLUDE_SDMMC					+= -I$(SAMLIB_PATH)/common/components/memory/sd_mmc
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb/class/cdc/device
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb/class/cdc
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb/udc
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb

INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/drivers

INCLUDE_BASE					= $(INCLUDE_CMSIS_CORE) $(INCLUDE_MCU_CORE) $(INCLUDE_SDMMC) $(INCLUDE_USB_STD)

CFLAG_INCLUDE                   = -I$(ITFDIR) $(INCLUDE_BASE) $(CONFIG_INCLUDE_DRIVER)
CFALG_MISC                      = -fomit-frame-pointer -Wall -fmessage-length=0 -c

CFLAGS                          = $(CFLAG_ARCH) $(CFALG_OPTIMIZATION) $(CFALG_DEBUG) $(CFLAG_INCLUDE) $(CFALG_MISC) $(CFLAG_SAM)
CXXFLAGS                        = $(CFLAGS)
ASFLAGS                         = $(CFLAGS)

ARFLAGS                         = rcs
LDFLAGS                         = -mcpu=cortex-m4 -nostartfiles -nostdlib -mfloat-abi=softfp -mfpu=fpv4-sp-d16

AS_entry                        = $(AS)
ASFLAGS_entry                   = $(ASFLAGS) -mthumb -D__ASSEMBLY__

CC_entry                        = $(CC)
CFLAGS_entry                    = $(CFLAGS) $(CFLAG_OPSTATE)

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

OBJS_entry                      = 

OBJS                            = 
DEPS                            = 
LIBS                            = 

################

TARGET                          = bsp_armcortexm_sam4eek

ASSRCS_entry                   += 
ASOBJS_entry                   += $(patsubst %.S,%.o, $(ASSRCS_entry))
$(ASOBJS_entry) : %.o: $(SRCDIR)/%.S
	@$(MKDIR) $(dir $@)
	$(AS_entry) $(ASFLAGS_entry) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS_entry                     += $(ASOBJS_entry)
DEPS                           += $(patsubst %.o,%.d, $(ASOBJS_entry))

CSRCS_entry                    += board/cstartup.c
COBJS_entry                    += $(patsubst %.c,%.o, $(CSRCS_entry))
$(COBJS_entry) : %.o: $(SRCDIR)/%.c
	$(CC_entry) $(CFLAGS_entry) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS_entry                     += $(COBJS_entry)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_entry))

CSRCS_default                  += common/main.c common/bsp.c arch/bsp_arch.c board/bsp_board.c board/dtty.c board/intr.c board/bsp_fpu.c
COBJS_default                  += $(patsubst %.c,%.o, $(CSRCS_default))
$(COBJS_default) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_default) $(CFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_default)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_default))

CSRCS__arm                     += arch/bsp_arch__arm.c
COBJS__arm                     += $(patsubst %.c,%.o, $(CSRCS__arm))
$(COBJS__arm) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC__arm) $(CFLAGS__arm) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS__arm)
DEPS                           += $(patsubst %.o,%.d, $(COBJS__arm))

CSRCS_nosocheck                += arch/bsp_arch_nosocheck.c  board/dtty_nosocheck.c
COBJS_nosocheck                += $(patsubst %.c,%.o, $(CSRCS_nosocheck))
$(COBJS_nosocheck) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_nosocheck) $(CFLAGS_nosocheck) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_nosocheck)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_nosocheck))
								
CSRCS_nosocheck__arm           += common/bsp_nosocheck__arm.c arch/bsp_arch_nosocheck__arm.c board/bsp_board_nosocheck__arm.c board/lowlevel__arm.c
COBJS_nosocheck__arm           += $(patsubst %.c,%.o, $(CSRCS_nosocheck__arm))
$(COBJS_nosocheck__arm) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_nosocheck__arm) $(CFLAGS_nosocheck__arm) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_nosocheck__arm)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_nosocheck__arm))

CSRCS_nosocheck__thumb         += common/bsp_nosocheck__thumb.c
COBJS_nosocheck__thumb         += $(patsubst %.c,%.o, $(CSRCS_nosocheck__thumb))
$(COBJS_nosocheck__thumb) : %.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC_nosocheck__thumb) $(CFLAGS_nosocheck__thumb) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_nosocheck__thumb)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_nosocheck__thumb))

################

.PHONY : all config compile link clean

################

all: config compile link
	@echo "\n[$$(pwd) : make "$@"]"

config:
	@echo "\n[$$(pwd) : make "$@"]"

build: compile link
	@echo "\n[$$(pwd) : make "$@"]"

compile: $(OBJS) $(OBJS_entry)
	@echo "\n[$$(pwd) : make "$@"]"

link: $(OBJS) $(OBJS_entry)
	@echo "\n[$$(pwd) : make "$@"]"
	$(AR) $(ARFLAGS) lib$(TARGET_TOOLCHAIN_TYPE)__$(TARGET).a $(OBJS)

clean:
	@echo "\n[$$(pwd) : make "$@"]"
	$(RM) $(OBJS) $(OBJS_entry)
	$(RM) $(DEPS)
	$(RM) lib$(TARGET_TOOLCHAIN_TYPE)__$(TARGET).a

################

-include $(DEPS)

################
