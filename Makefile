TARGET ?= pltos
CROSS_COMPILE ?= arm-none-eabi-
V ?= 0
DEBUG ?= 1

MAKEFLAGS += --no-print-directory

srctree := .
export srctree

ifeq ($(V), 1)
	quiet=
	Q =
else
	quiet=quiet_
	Q = @
endif

export quiet Q

USER_INCLUDE  := \
			-include include/plt/kconfig.h \
			-Iinclude

USER_ASFLAGS  :=

USER_CFLAGS  := \
			-std=gnu99

USER_CXXFLAGS :=

# --cref 在映射文件中生成符号的交叉引用信息，显示函数之间的调用关系
# --gc-sections 链接器将执行无用代码消除
USER_LDFLAGS  := \
			-Wl,-Map,$(TARGET).map,--cref -Wl,-Tlink.lds -Wl,--gc-sections

-include arch/cortex-m3/Makefile

BUILD_CFLAGS   := $(ARCH_CFLAGS) $(USER_CFLAGS) $(USER_INCLUDE)
BUILD_ASFLAGS  := $(ARCH_ASFLAGS) $(USER_ASFLAGS)
BUILD_CXXFLAGS := $(ARCH_CXXFLAGS) $(USER_CXXFLAGS) $(USER_INCLUDE)
BUILD_LDFLAGS  := $(ARCH_LDFLAGS) $(USER_LDFLAGS)

ifeq ($(DEBUG), 1)
CFLAGS += -g
else
CFLAGS += -Os
endif
ASFLAGS  += $(BUILD_ASFLAGS)
CFLAGS   += $(BUILD_CFLAGS) -Wall
CXXFLAGS += $(BUILD_CXXFLAGS) -Wall
LDFLAGS  += $(BUILD_LDFLAGS)

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
SIZE        = $(CROSS_COMPILE)size

OCD         = openocd

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP
export MAKE MAKEFLAGS
export CFLAGS CXXFLAGS LDFLAGS
export CHECK CHECKFLAGS

include scripts/Build.include

targets := $(wildcard $(sort $(targets)))
cmd_files := $(wildcard .*.cmd $(foreach f,$(targets),$(dir $(f)).$(notdir $(f)).cmd))

ifneq ($(cmd_files),)
  include $(cmd_files)
endif

PHONY += all clean size

all: $(TARGET).bin

objs-y += kernel app arch drivers init libs mm
libs-y +=

sys-libs    := $(libs-y)
sys-dirs    := $(objs-y)
sys-objs	:= $(patsubst %,%/built-in.o, $(objs-y))
sys-all     := $(sys-objs) $(sys-libs)


quiet_cmd_sys = LD    $@
cmd_sys = $(CC) $(LDFLAGS) -o $@ \
		  -Wl,--start-group $(sys-all) -Wl,--end-group

$(TARGET).elf: $(sys-objs) FORCE
	+$(call if_changed,sys)

quiet_cmd_cphex = OBJCOPY    $@
cmd_cphex = $(OBJCOPY) -O ihex $< $@;$(MAKE) size

%.hex: %.elf FORCE
	+$(call if_changed,cphex)

quiet_cmd_cpbin = OBJCOPY    $@
cmd_cpbin = $(OBJCOPY) -O binary -S $< $@;$(MAKE) size

%.bin: %.elf FORCE
	+$(call if_changed,cpbin)

quiet_cmd_size = SIZE    $(TARGET).elf
cmd_size = $(SIZE) $(TARGET).elf

size:
	+$(call cmd,size)

$(sort $(sys-objs)): $(sys-dirs) ;

PHONY += $(sys-dirs)
$(sys-dirs):
	$(Q)$(MAKE) $(build)=$@

rm-files = $(wildcard $(TARGET).bin $(TARGET).hex $(TARGET).elf $(TARGET).map)

quiet_cmd_rmfiles := $(if $(rm-files),CLEAN    $(rm-files))
cmd_rmfiles := $(RM) -r $(rm-files)

clean:
	$(call cmd,rmfiles)
	$(Q)$(RM) -r $(shell find -name *.o -o -name '.*.cmd')

PHONY += flash

flash: $(TARGET).bin
	$(OCD) -f scripts/openocd.cfg -c "program"

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

