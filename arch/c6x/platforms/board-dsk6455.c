/*
 *  linux/arch/c6x/platforms/board-dsk6455.c
 *
 *  Port on Texas Instruments TMS320C6x architecture
 *
 *  Copyright (C) 2008, 2009, 2010, 2011 Texas Instruments Incorporated
 *  Author: Aurelien Jacquiot (aurelien.jacquiot@virtuallogix.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/netdevice.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c/at24.h>
#include <linux/kernel_stat.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/machdep.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/timer.h>
#include <asm/percpu.h>
#include <asm/clock.h>
#include <asm/edma.h>
#include <asm/dscr.h>

#include <mach/board.h>
#include <mach/emif.h>

/*
 * Resources present on the DSK6455 board
 */
static struct resource _flash_res = {
	.name  = "Flash",
	.start = 0xb0000000,
	.end   = 0xbfffffff,
	.flags = IORESOURCE_MEM,
};
static struct resource _cpld_async_res = {
	.name  = "CPLD async",
	.start = 0xa0000000,
	.end   = 0xa0000008,
	.flags = IORESOURCE_MEM,
};

#define NR_RESOURCES 2
static struct resource *dsk_resources[NR_RESOURCES] = 
	{ &_flash_res, &_cpld_async_res };


/*----------------------------------------------------------------------*/

#ifdef CONFIG_EDMA3

/* Four Transfer Controllers on TMS320C6455 */
static const s8
queue_tc_mapping[][2] = {
	/* {event queue no, TC no} */
	{0, 0},
	{1, 1},
	{2, 2},
	{3, 3},
	{-1, -1},
};

static const s8
queue_priority_mapping[][2] = {
	/* {event queue no, Priority} */
	{0, 4},	/* FIXME: what should these priorities be? */
	{1, 0},
	{2, 5},
	{3, 1},
	{-1, -1},
};


static struct edma_soc_info edma_cc0_info = {
	.n_channel		= EDMA_NUM_DMACH,
	.n_region		= EDMA_NUM_REGIONS,
	.n_slot			= EDMA_NUM_PARAMENTRY,
	.n_tc			= EDMA_NUM_EVQUE,
	.n_cc			= 1,
	.queue_tc_mapping	= queue_tc_mapping,
	.queue_priority_mapping	= queue_priority_mapping,
};

static struct edma_soc_info *edma_info[] = {
	&edma_cc0_info,
};

static struct resource edma_resources[] = {
	{
		.name	= "edma_cc0",
		.start	= EDMA_REGISTER_BASE,
		.end	= EDMA_REGISTER_BASE + 0xFFFF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc0",
		.start	= EDMA_TC0_BASE,
		.end	= EDMA_TC0_BASE + 0x03FF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc1",
		.start	= EDMA_TC1_BASE,
		.end	= EDMA_TC1_BASE + 0x03FF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc2",
		.start	= EDMA_TC2_BASE,
		.end	= EDMA_TC2_BASE + 0x03FF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc3",
		.start	= EDMA_TC3_BASE,
		.end	= EDMA_TC3_BASE + 0x03FF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma0",
		.start	= EDMA_IRQ_CCINT,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "edma0_err",
		.start	= EDMA_IRQ_CCERRINT,
		.flags	= IORESOURCE_IRQ,
	},
	/* not using TC*_ERR */
};

static struct platform_device edma_device = {
	.name			= "edma",
	.id			= 0,
	.dev.platform_data	= edma_info,
	.num_resources		= ARRAY_SIZE(edma_resources),
	.resource		= edma_resources,
};


static void __init dsk_setup_edma(void)
{
	int status;

	status = platform_device_register(&edma_device);
	if (status != 0)
		pr_debug("setup_edma --> %d\n", status);
}
#else
#define dsk_setup_edma()
#endif /* CONFIG_EDMA3 */

/*----------------------------------------------------------------------*/


#ifdef CONFIG_I2C
static struct at24_platform_data at24_eeprom_data = {
	.byte_len	= 0x100000 / 8,
	.page_size	= 256,
	.flags		= AT24_FLAG_ADDR16,
};

static struct i2c_board_info dsk_i2c_info[] = {
#ifdef CONFIG_EEPROM_AT24
	{ I2C_BOARD_INFO("24c1024", 0x50),
	  .platform_data = &at24_eeprom_data,
	},
#endif
};

static void __init dsk_setup_i2c(void)
{
	i2c_register_board_info(1, dsk_i2c_info,
				ARRAY_SIZE(dsk_i2c_info));
}
#else
#define dsk_setup_i2c()
#endif /* CONFIG_I2C */

SOC_CLK_DEF(50000000);  /* clkin is a 50 MHz clock */

static struct clk_lookup evm_clks[] = {
	SOC_CLK(),
	CLK("", NULL, NULL)
};

static void dummy_print_dummy(char *s, unsigned long hex) {}
static void dummy_progress(unsigned int step, char *s) {}

/* Called from arch/kernel/setup.c */
void c6x_board_setup_arch(void)
{   
	int i;

	printk("Designed for the DSK6455 board, Spectrum Digital Inc.\n");

	/* Bootloader may not have setup EMIFA, so we do it here just in case */
	dscr_set_reg(DSCR_PERCFG1, 3);
	__delay(100);

	/* CPLD */
	EMIFA_CE2CFG = EMIFA_CFG_ASYNC |
		       EMIFA_CFG_W_SETUP(1)   |
		       EMIFA_CFG_W_STROBE(10) |
		       EMIFA_CFG_W_HOLD(1)    |
		       EMIFA_CFG_R_SETUP(1)   |
		       EMIFA_CFG_R_STROBE(10) |
		       EMIFA_CFG_R_HOLD(1)    |
		       EMIFA_CFG_WIDTH_8;

	/* NOR Flash */
	EMIFA_CE3CFG = EMIFA_CFG_ASYNC |
		       EMIFA_CFG_W_SETUP(1)   |
		       EMIFA_CFG_W_STROBE(10) |
		       EMIFA_CFG_W_HOLD(1)    |
		       EMIFA_CFG_R_SETUP(1)   |
		       EMIFA_CFG_R_STROBE(10) |
		       EMIFA_CFG_R_HOLD(1)    |
		       EMIFA_CFG_WIDTH_8;

	/* Daughter Card */
	EMIFA_CE4CFG = EMIFA_CFG_ASYNC |
		       EMIFA_CFG_W_SETUP(1)   |
		       EMIFA_CFG_W_STROBE(10) |
		       EMIFA_CFG_W_HOLD(1)    |
		       EMIFA_CFG_R_SETUP(1)   |
		       EMIFA_CFG_R_STROBE(10) |
		       EMIFA_CFG_R_HOLD(1)    |
		       EMIFA_CFG_WIDTH_32;

	/* Daughter Card */
	EMIFA_CE5CFG = EMIFA_CFG_ASYNC |
		       EMIFA_CFG_W_SETUP(1)   |
		       EMIFA_CFG_W_STROBE(10) |
		       EMIFA_CFG_W_HOLD(1)    |
		       EMIFA_CFG_R_SETUP(1)   |
		       EMIFA_CFG_R_STROBE(10) |
		       EMIFA_CFG_R_HOLD(1)    |
		       EMIFA_CFG_WIDTH_32;

	/* Raise priority of waiting bus commands after 255 transfers */
	EMIFA_BPRIO = 0xFE;

	/* Initialize DSK6455 resources */
	iomem_resource.name = "Memory";
	for (i = 0; i < NR_RESOURCES; i++)
		request_resource(&iomem_resource, dsk_resources[i]);

	/* Initialize led register */
	cpld_set_reg(DSK6455_CPLD_USER, 0x0);

	mach_progress      = dummy_progress;
	mach_print_value   = dummy_print_dummy;

	c6x_clk_init(evm_clks);

	mach_progress(1, "End of DSK6455 specific initialization");
}

static int __init evm_init(void)
{
	dsk_setup_i2c();
	dsk_setup_edma();
        return 0;
}

arch_initcall(evm_init);

/*
 * NOR Flash support.
 */
#ifdef CONFIG_MTD
#ifndef CONFIG_TI_C6X_COMPILER /* toolchain bug compiling cfi_amdstd_setup() */
static struct map_info nor_map = {
	.name		= "NOR-flash",
	.phys		= 0xB0000000,
	.size		= 0x400000,
	.bankwidth	= 1,
};
static struct mtd_info *mymtd;
#ifdef CONFIG_MTD_PARTITIONS
static int nr_parts;
static struct mtd_partition *parts;
static const char *part_probe_types[] = {
	"cmdlinepart",
	NULL
};
#endif

static __init int nor_init(void)
{
	nor_map.virt = ioremap(nor_map.phys, nor_map.size);
	simple_map_init(&nor_map);
	mymtd = do_map_probe("cfi_probe", &nor_map);
	if (mymtd) {
		mymtd->owner = THIS_MODULE;

#ifdef CONFIG_MTD_PARTITIONS
		nr_parts = parse_mtd_partitions(mymtd,
						part_probe_types,
						&parts, 0);
		if (nr_parts > 0)
			add_mtd_partitions(mymtd, parts, nr_parts);
		else
			add_mtd_device(mymtd);
#else
		add_mtd_device(mymtd);
#endif
	}
	return 0;
}

late_initcall(nor_init);
#endif /* CONFIG_TI_C6X_COMPILER */
#endif
