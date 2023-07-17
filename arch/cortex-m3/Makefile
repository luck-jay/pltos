CPU = -mcpu=cortex-m3
FPU =
MCU = $(CPU) $(FPU)

# -mthumb 是一种32位RISC指令集的压缩格式，可以显著减小代码大小，提高代码密度
# nosys.specs 规范可以生成与特定操作系统无关的可执行文件
# nano.specs 规范是一个精简版的规范文件，定义了一个小型的c runingtime，
# 旨在减小可执行文件大小
# -fdata-sections 编译器将每个全局变量和静态变量放置在单独的数据段中
# -ffunction-sections 编译器将每个函数放置在单独的代码段中
# -fno-common 这个选项告诉编译器禁止使用通用的存储类定义（如未初始化的全局变量）
FLAGS = -mthumb \
		-fdata-sections -ffunction-sections \
		--specs=nosys.specs --specs=nano.specs
#        -fno-common

arch-cflags-$(CONFIG_STM32_HIGH_CAPACITY) = -DSTM32F10X_HD
arch-cflags-$(CONFIG_STM32_MID_CAPACITY) = -DSTM32F10X_MD
arch-cflags-$(CONFIG_STM32_LOW_CAPACITY) = -DSTM32F10X_MD

ARCH_ASFLAGS  := $(MCU) $(FLAGS)

ARCH_CFLAGS   := $(MCU) $(FLAGS) $(arch-cflags-y) -Iarch/cortex-m3/include
# -flto 允许编译器在链接过程中对整个程序进行优化，包括多个源文件之间的优化
ARCH_LDFLAGS  := $(FLAGS) # -flto

ccflags-y = -Iinclude/rtos

obj-y += startup/
obj-y += stm32f1x/
obj-y += port.o
