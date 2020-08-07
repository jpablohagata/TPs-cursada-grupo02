ifeq ($(USE_BOARD),y)

BOARD=edu_ciaa_nxp

DEFINES+=__USE_LPCOPEN
DEFINES+=CHIP_LPC43XX
DEFINES+=ARM_MATH_CM4

BOARD_BASE=libs/board

SRC+=$(wildcard $(BOARD_BASE)/$(BOARD)/src/*.c)
INCLUDES+=-I$(BOARD_BASE)/$(BOARD)/inc

SRC+=$(wildcard $(BOARD_BASE)/lpc_chip_43xx/src/*.c)

INCLUDES+=-I$(BOARD_BASE)/lpc_chip_43xx/inc
INCLUDES+=-I$(BOARD_BASE)/lpc_chip_43xx/usbd_rom

endif

DEFINES+=CORE_M4 __USE_NEWLIB
ARCH_FLAGS=-mcpu=cortex-m4 -mthumb

ifeq ($(USE_FPU),y)
DEFINES+=__FPU_PRESENT=1U
ARCH_FLAGS+=-mfloat-abi=hard -mfpu=fpv4-sp-d16
else
DEFINES+=__FPU_PRESENT=OU
endif

ifeq ($(LOAD_INRAM),y)
LDSCRIPT=flat.ld
else
LDSCRIPT=link.ld
endif

SRC+=$(wildcard $(BOARD_BASE)/lpc_startup/src/*.c) 
INCLUDES+=-I$(BOARD_BASE)/lpc_startup/inc
