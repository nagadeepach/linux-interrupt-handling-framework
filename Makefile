#===========================================================
# Makefile
# Linux Interrupt Handling Framework
# This Makefile is providing convenient targets for building, loading, unloading, logging, and verifying the interrupt framework module.
#===========================================================

# Module Name
obj-m += interrupt_driver.o

# Kernel Build Directory
KDIR := /lib/modules/$(shell uname -r)/build

# Current Directory
PWD := $(shell pwd)

#===========================================================
# Build Module
#===========================================================

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

#===========================================================
# Clean Build Files
#===========================================================

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

#===========================================================
# Load Module
#===========================================================

load:
	sudo insmod interrupt_driver.ko

#===========================================================
# Unload Module
#===========================================================

unload:
	sudo rmmod interrupt_driver

#===========================================================
# Show Kernel Messages
#===========================================================

log:
	dmesg | tail -30

#===========================================================
# Verify IRQ Registration
#===========================================================

verify:
	cat /proc/interrupts | grep -E "i8042|interrupt_framework"

#===========================================================
#===========================================================
