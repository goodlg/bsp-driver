# a sample platform driver/(miscdevice) 
    1. kernel/msm-4.4/arch/arm64/configs/xxx_defconfig
        CONFIG_FELLOWPLAT=m
    2. kernel/msm-4.4/drivers/input/misc/Kconfig
         config FELLOWPLAT 
            tristate "a sample platform driver"
            depends on INPUT
            help
              a sample platform driver code.
    3. kernel/msm-4.4/drivers/input/misc/Makefile
         obj-$(CONFIG_FELLOWPLAT) += gyniu/
