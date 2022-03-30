SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(SELF_DIR)/../../src.mk

ifdef PULPOS_BOARD
BOARD_UPPER = $(shell echo $(PULPOS_BOARD) | tr 'a-z' 'A-Z')
PULP_CFLAGS += -DCONFIG_$(BOARD_UPPER)
endif

ifdef PULPOS_BOARD_VERSION
BOARD_VERSION_UPPER = $(shell echo $(PULPOS_BOARD_VERSION) | tr 'a-z' 'A-Z')
PULP_CFLAGS += -DCONFIG_BOARD_VERSION_$(BOARD_VERSION_UPPER)
endif

ifndef PULPOS_BOARD_PROFILE
PULPOS_BOARD_PROFILE = default
endif

BOARD_PROFILE_UPPER = $(shell echo $(PULPOS_BOARD_PROFILE) | tr 'a-z' 'A-Z')
PULP_CFLAGS += -DCONFIG_PROFILE_$(BOARD_PROFILE_UPPER)

# BSP is needed if i2s is used to properly configure pads
ifeq '$(CONFIG_I2S)' '1'
CONFIG_BSP = 1
endif

ifeq '$(CONFIG_24XX1025)' '1'
PULP_SRCS += $(BSP_24XX1025_SRC)
CONFIG_BSP = 1
CONFIG_I2C = 1
endif

ifeq '$(CONFIG_VIRTUAL_EEPROM)' '1'
PULP_SRCS += $(BSP_VIRTUAL_EEPROM_SRC)
CONFIG_BSP = 1
CONFIG_I2C = 1
endif

ifeq '$(CONFIG_SPIRAM)' '1'
PULP_SRCS += $(BSP_RAM_SRC) $(BSP_SPIRAM_SRC) ram/spiram/aps25xxxn.c
CONFIG_BSP = 1
CONFIG_SPIM = 1
endif

ifeq '$(CONFIG_HYPERRAM)' '1'
PULP_SRCS += $(BSP_RAM_SRC) $(BSP_HYPERRAM_SRC)
CONFIG_BSP = 1
CONFIG_HYPER = 1
endif

ifeq '$(CONFIG_READFS)' '1'
PULP_SRCS += $(BSP_READFS_SRC)
CONFIG_FS = 1
endif

ifeq '$(CONFIG_LFS)' '1'
PULP_SRCS += $(BSP_LFS_SRC)
CONFIG_FS = 1
endif

ifeq '$(CONFIG_HOSTFS)' '1'
PULP_SRCS += $(BSP_HOSTFS_SRC)
CONFIG_FS = 1
endif

ifeq '$(CONFIG_FS)' '1'
PULP_SRCS += $(BSP_FS_SRC)
CONFIG_BSP = 1
endif

ifeq '$(CONFIG_HYPERFLASH)' '1'
PULP_SRCS += $(BSP_HYPERFLASH_SRC)
CONFIG_FLASH = 1
endif

ifeq '$(CONFIG_MRAM)' '1'
PULP_SRCS += $(BSP_MRAM_SRC)
CONFIG_FLASH = 1
endif

ifeq '$(CONFIG_SPIFLASH)' '1'
PULP_SRCS += $(BSP_SPIFLASH_SRC)
CONFIG_FLASH = 1
endif

ifeq '$(CONFIG_ATXP032)' '1'
PULP_SRCS += flash/spiflash/atxp032.c
CONFIG_FLASH = 1
CONFIG_HYPER = 1
endif

ifeq '$(CONFIG_FLASH)' '1'
PULP_SRCS += $(BSP_FLASH_SRC)
CONFIG_BSP = 1
endif

ifeq '$(CONFIG_HIMAX)' '1'
CONFIG_CAMERA = 1
CONFIG_I2C = 1
PULP_SRCS += $(BSP_HIMAX_SRC)
endif

ifeq '$(CONFIG_CAMERA)' '1'
PULP_SRCS += $(BSP_CAMERA_SRC)
endif

ifeq '$(CONFIG_BSP)' '1'
PULP_SRCS += bsp/$(PULPOS_BOARD).c
endif

PULP_SRCS += $(PMSIS_BSP_SRC)
PULP_CFLAGS += $(PMSIS_BSP_CFLAGS)