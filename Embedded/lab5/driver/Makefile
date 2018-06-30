ifeq ($(KERNELRELEASE),)

GCC_DIR:="../../../tools/arm-2009q3/bin"
KDIR:="../../../linux-3.0.15"

ifneq ($(shell if [ -d "$(GCC_DIR)" ]; then echo true; fi),true)
$(error GCC_DIR=$(GCC_DIR) does not exist)
endif

ifneq ($(shell if [ -d "$(KDIR)" ]; then echo true; fi),true)
$(error KDIR=$(KDIR) does not exist)
endif

GCC_DIR:=$(shell cd $(GCC_DIR) && pwd)
KDIR:=$(shell cd $(KDIR) && pwd)

CROSS_COMPILE:=$(GCC_DIR)/arm-none-linux-gnueabi-

PWD:=$(shell pwd)

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.* modules.*

.PHONY: modules clean

else

obj-m:=s3c-leds.o

endif

all:
	adb push s3c-leds.ko /data/local
	adb shell insmod /data/local/s3c-leds.ko
