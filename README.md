# Linux Interrupt Handling Framework

## Hands-on Linux Kernel Interrupt Handling using ISR, Shared IRQ, Workqueues, verification, debugging, and device driver development best practices.

### Overview
This project demonstrates Linux interrupt handling using:
- request_irq()
- IRQF_SHARED
- Interrupt Service Routine (ISR)
- Workqueue (Bottom Half)
- Deferred Processing
- Verification using /proc/interrupts and dmesg

> Note: IRQ1 (keyboard) is used only for educational demonstration.

## Architecture

Keyboard Controller -> Interrupt Controller -> Linux Interrupt Core -> request_irq() -> ISR -> schedule_work() -> Workqueue -> Deferred Processing

## Build
```bash
make
```

## Load
```bash
sudo insmod interrupt_driver.ko
```

## Verify
```bash
cat /proc/interrupts
dmesg | tail -20
```

## Remove
```bash
sudo rmmod interrupt_driver
```

## APIs
- request_irq()
- free_irq()
- INIT_WORK()
- schedule_work()

## Author
**Deepa**
