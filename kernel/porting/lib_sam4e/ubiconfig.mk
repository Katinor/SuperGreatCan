#lib_sam4e
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
CFLAG_INCLUDE                   = -I$(ITFDIR)
CFALG_MISC                      = -fomit-frame-pointer -Wall -fmessage-length=0 -c

######################################################################
SAMLIB_PATH						= $(ITFDIR)/../kernel/porting/lib_sam4e

INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/cfg
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/boards
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/boards/user_board
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/boards/user_board/board_config
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/components
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/drivers
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/services/clock
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/services/clock/sam4e/module_config
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/services/sleepmgr
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/services/ioport
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common/utils
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/common2
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/thirdparty/CMSIS/Include
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/cmsis/sam4e/include
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/cmsis/sam4e/include/componet
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/cmsis/sam4e/include/instance
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/cmsis/sam4e/include/pio
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/cmsis/sam4e/source/templates
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/fpu
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/header_files
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/syscalls/gcc
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/utils/preprocessor
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/boards/sam4e_ek
INCLUDE_MCU_CORE				+= -I$(SAMLIB_PATH)/sam/drivers/pmc
######################################################################
				
INCLUDE_SDMMC					+= -I$(SAMLIB_PATH)/common/components/memory/sd_mmc
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb/class/cdc/device
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb/class/cdc
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb/udc
INCLUDE_USB_STD					+= -I$(SAMLIB_PATH)/common/services/usb

INCLUDE_MCU_OPTION				+= $(INCLUDE_SDMMC) $(INCLUDE_USB_STD)

######################################################################

INCLUDE_MCU_DRIVER				+= -I$(SAMLIB_PATH)/sam/drivers
INCLUDE_MCU_CORE				+= -I$(ITFDIR)/../kernel/porting/lib_sam4e/sam/drivers/uart #lowpower

######################################################################

CFLAG_SAM						+= -D__SAM4E16E__
CFLAG_SAM						+= -DBOARD=SAM4E_EK
CFLAG_SAM						+= -std=c99

SAM_OPTION						+= $(CFLAG_SAM) $(INCLUDE_MCU_CORE) $(INCLUDE_MCU_OPTION) $(INCLUDE_ADD) $(INCLUDE_MCU_DRIVER)

######################################################################
CFLAGS                          = $(CFLAG_ARCH) $(CFALG_OPTIMIZATION) $(CFALG_DEBUG) $(CFLAG_INCLUDE) $(CFALG_MISC) $(SAM_OPTION)
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
CFLAGS_nosocheck                = $(CFLAGS) $(CFLAG_OPSTATE)

CC_nosocheck__arm               = $(CC)
CFLAGS_nosocheck__arm           = $(CFLAGS) -mthumb

CC_nosocheck__thumb             = $(CC)
CFLAGS_nosocheck__thumb         = $(CFLAGS) -mthumb

################

OBJS_entry                      = 

OBJS                            = 
DEPS                            = 
LIBS                            = 

################

TARGET                          = lib_sam4e

CSRCS_default                  +=  common/utils/interrupt/interrupt_sam_nvic.c
CSRCS_default                  +=  common/services/clock/sam4e/sysclk.c
#CSRCS_default                  +=  common/services/spi/sam_usart_spi/usart_spi.c
CSRCS_default                  +=  common/services/spi/sam_spi/spi_master.c
CSRCS_default                  +=  sam/utils/cmsis/sam4e/source/templates/exceptions.c
CSRCS_default                  +=  sam/utils/cmsis/sam4e/source/templates/system_sam4e.c

CSRCS_default                  +=  sam/drivers/pmc/pmc.c

CSRCS_default                  +=  sam/drivers/acc/acc.c
CSRCS_default                  +=  sam/drivers/aes/aes.c
CSRCS_default                  +=  sam/drivers/afec/afec.c
CSRCS_default                  +=  sam/drivers/can/can.c
CSRCS_default                  +=  sam/drivers/chipid/chipid.c
CSRCS_default                  +=  sam/drivers/cmcc/cmcc.c
CSRCS_default                  +=  sam/drivers/dacc/dacc.c
CSRCS_default                  +=  sam/drivers/dmac/dmac.c
CSRCS_default                  +=  sam/drivers/efc/efc.c
CSRCS_default                  +=  sam/drivers/gmac/gmac_phy.c
CSRCS_default                  +=  sam/drivers/gmac/gmac_raw.c
CSRCS_default                  +=  sam/drivers/gpbr/gpbr.c
CSRCS_default                  +=  sam/drivers/hsmci/hsmci.c
CSRCS_default                  +=  sam/drivers/matrix/matrix.c
CSRCS_default                  +=  sam/drivers/pdc/pdc.c
CSRCS_default                  +=  sam/drivers/pio/pio.c
CSRCS_default                  +=  sam/drivers/pio/pio_handler.c

CSRCS_default                  +=  sam/drivers/pmc/sleep.c
CSRCS_default                  +=  sam/drivers/rstc/rstc.c
CSRCS_default                  +=  sam/drivers/rtc/rtc.c
CSRCS_default                  +=  sam/drivers/rtt/rtt.c
CSRCS_default                  +=  sam/drivers/spi/spi.c
CSRCS_default                  +=  sam/drivers/supc/supc.c
CSRCS_default                  +=  sam/drivers/tc/tc.c
CSRCS_default                  +=  sam/drivers/twi/twi.c
CSRCS_default                  +=  sam/drivers/uart/uart.c
CSRCS_default                  +=  sam/drivers/udp/udp_device.c
CSRCS_default                  +=  sam/drivers/usart/usart.c
CSRCS_default                  +=  sam/drivers/wdt/wdt.c

CSRCS_default                  +=  thirdparty/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_cos_f32.c
CSRCS_default                  +=  thirdparty/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_f32.c

CSRCS_default					+= common/utils/stdio/read.c
CSRCS_default					+= common/utils/stdio/write.c

CSRCS_default					+= sam/applications/sam_low_power/sam4e16e_sam4e_ek/low_power_board.c
#CSRCS_default                  +=  sam/drivers/wdt/wdt_sam4l.c

COBJS_default                  += $(patsubst %.c,%.o, $(CSRCS_default))
$(COBJS_default) : %.o: $(SRCDIR)/../%.c
	@$(MKDIR) $(dir $@)
	$(CC_default) $(CFLAGS_default) -MD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<"	
OBJS                           += $(COBJS_default)
DEPS                           += $(patsubst %.o,%.d, $(COBJS_default))

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
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) lib$(TARGET_TOOLCHAIN_TYPE)__$(TARGET).a

################

-include $(DEPS)

################
