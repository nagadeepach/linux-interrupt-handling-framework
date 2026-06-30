/*
 * interrupt_driver.c
 *
 * Linux Interrupt Handling Framework
 * ----------------------------------
 * Demonstrates:
 *  - request_irq()
 *  - Shared IRQ (IRQF_SHARED)
 *  - Interrupt Service Routine (Top Half)
 *  - Workqueue (Bottom Half)
 *  - Deferred Processing
 *  - Interrupt Statistics
 *  - Proper Cleanup
 *
 * Platform : Ubuntu Linux (x86)
 *
 * Author : Deepa
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/smp.h>
#include <linux/jiffies.h>
#include <linux/sched.h>

/*-----------------------------------------------------------*/
/* Module Parameter                                           */
/*-----------------------------------------------------------*/

static int irq = 1;     /* Keyboard IRQ (Demo only) */

module_param(irq, int, 0444);
MODULE_PARM_DESC(irq, "IRQ Number");

/*-----------------------------------------------------------*/
/* Global Variables                                           */
/*-----------------------------------------------------------*/

static struct work_struct irq_work;

static unsigned long interrupt_count;

/*-----------------------------------------------------------*/
/* Bottom Half (Workqueue)                                    */
/*-----------------------------------------------------------*/

static void workqueue_function(struct work_struct *work)
{
    pr_info("\n");
    pr_info("=============== BOTTOM HALF =================\n");
    pr_info("Running in Process Context\n");
    pr_info("Current Process : %s\n", current->comm);
    pr_info("Deferred Processing Completed\n");
    pr_info("=============================================\n");
}

/*-----------------------------------------------------------*/
/* Interrupt Service Routine   (Top Half)                               */
/*-----------------------------------------------------------*/

static irqreturn_t irq_handler(int irq, void *dev_id)
{
    interrupt_count++;

    pr_info("\n");
    pr_info("**************** TOP HALF *******************\n");
    pr_info("Interrupt Received\n");
    pr_info("IRQ Number       : %d\n", irq);
    pr_info("CPU              : %u\n", smp_processor_id());
    pr_info("Interrupt Count  : %lu\n", interrupt_count);

    if (!work_pending(&irq_work)) {
        schedule_work(&irq_work);
        pr_info("Bottom Half Scheduled\n");
    }

    pr_info("*********************************************\n");

    return IRQ_HANDLED;
}

/*-----------------------------------------------------------*/
/* Module Initialization                                      */
/*-----------------------------------------------------------*/

static int __init interrupt_driver_init(void)
{
    int ret;

    pr_info("\n");
    pr_info("=============================================\n");
    pr_info(" Linux Interrupt Handling Framework\n");
    pr_info("=============================================\n");

    interrupt_count = 0;

    INIT_WORK(&irq_work, workqueue_function);

    ret = request_irq(irq,
                      irq_handler,
                      IRQF_SHARED,
                      "interrupt_framework",
                      &irq);

    if (ret) {
        pr_err("Failed to register IRQ %d\n", irq);
        return ret;
    }

    pr_info("Driver Loaded Successfully\n");
    pr_info("Registered Shared IRQ : %d\n", irq);
    pr_info("Waiting for Keyboard Interrupt...\n");
    pr_info("=============================================\n");

    return 0;
}

/*-----------------------------------------------------------*/
/* Module Exit                                                */
/*-----------------------------------------------------------*/

static void __exit interrupt_driver_exit(void)
{
    flush_work(&irq_work);

    free_irq(irq, &irq);

    pr_info("\n");
    pr_info("=============================================\n");
    pr_info("Interrupt Framework Driver Unloaded\n");
    pr_info("Total Interrupts Handled : %lu\n", interrupt_count);
    pr_info("IRQ Released Successfully\n");
    pr_info("=============================================\n");
}

/*-----------------------------------------------------------*/

module_init(interrupt_driver_init);
module_exit(interrupt_driver_exit);

/*-----------------------------------------------------------*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepa");
MODULE_DESCRIPTION("Linux Interrupt Handling Framework using ISR, Shared IRQ and Workqueue");
MODULE_VERSION("2.0");
