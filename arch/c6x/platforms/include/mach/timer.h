/*
 *  linux/arch/c6x/platforms/include/mach/timer.h
 *
 *  Timer definitions for Texas Instruments TMS320C6x architecture
 *
 *  Copyright (C) 2010, 2011 Texas Instruments Incorporated
 *  Author: Mark Salter <msalter@redhat.com>
 *          Aurelien Jacquiot <a-jacquiot@ti.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#ifndef __MACH_C6X_TIMER_H
#define __MACH_C6X_TIMER_H

/*
 * Timers register base
 */
#if defined(CONFIG_SOC_TMS320C6455) || defined(CONFIG_SOC_TMS320C6457)
#define TIMER_BASE_ADDR   0x02940000
#define TIMER_CHAN_MULT   0x00040000
#elif defined(CONFIG_SOC_TMS320C6472)
#define TIMER_BASE_ADDR   0x025e0000
#define TIMER_CHAN_MULT   0x00010000
#elif defined(CONFIG_SOC_TMS320C6474)
#define TIMER_BASE_ADDR   0x02910000
#define TIMER_CHAN_MULT   0x00010000
#define TIMER_TINPSEL_REG	0x02900000
#define TIMER_TOUTPSEL_REG	0x02900004
#define TIMER_WDRSTSEL_REG	0x02900008
#elif defined(CONFIG_SOC_TMS320C6670)
#define TIMER_BASE_ADDR   0x02200000
#define TIMER_CHAN_MULT   0x00010000
#elif defined(CONFIG_SOC_TMS320C6678)
#define TIMER_BASE_ADDR   0x02200000
#define TIMER_CHAN_MULT   0x00010000
#else
#error "no timer base defined"
#endif

/*
 * Timers management
 */
#define TIMER_BASE(chan)		(TIMER_BASE_ADDR + ((chan) * TIMER_CHAN_MULT))
#define TIMER_EMUMGTCLKSPD_REG(chan)	(TIMER_BASE(chan) + 0x04)
#define TIMER_CNTLO_REG(chan)		(TIMER_BASE(chan) + 0x10)
#define TIMER_CNTHI_REG(chan)		(TIMER_BASE(chan) + 0x14)
#define TIMER_PRDLO_REG(chan)		(TIMER_BASE(chan) + 0x18)
#define TIMER_PRDHI_REG(chan)		(TIMER_BASE(chan) + 0x1c)
#define TIMER_TCR_REG(chan)		(TIMER_BASE(chan) + 0x20)
#define TIMER_TGCR_REG(chan)		(TIMER_BASE(chan) + 0x24)
#define TIMER_WDTCR_REG(chan)		(TIMER_BASE(chan) + 0x28)

#define TIMER_B_TCR_TSTATLO          0x001
#define TIMER_B_TCR_INVOUTPLO        0x002
#define TIMER_B_TCR_INVINPLO         0x004
#define TIMER_B_TCR_CPLO             0x008
#define TIMER_B_TCR_ENAMODELO_ONCE   0x040
#define TIMER_B_TCR_ENAMODELO_CONT   0x080
#define TIMER_B_TCR_ENAMODELO_MASK   0x0c0
#define TIMER_B_TCR_PWIDLO_MASK      0x030
#define TIMER_B_TCR_CLKSRCLO         0x100
#define TIMER_B_TCR_TIENLO           0x200
#define TIMER_B_TCR_TSTATHI          (0x001 << 16)
#define TIMER_B_TCR_INVOUTPHI        (0x002 << 16)
#define TIMER_B_TCR_CPHI             (0x008 << 16)
#define TIMER_B_TCR_PWIDHI_MASK      (0x030 << 16)
#define TIMER_B_TCR_ENAMODEHI_ONCE   (0x040 << 16)
#define TIMER_B_TCR_ENAMODEHI_CONT   (0x080 << 16)
#define TIMER_B_TCR_ENAMODEHI_MASK   (0x0c0 << 16)

#define TIMER_B_TGCR_TIMLORS         0x001
#define TIMER_B_TGCR_TIMHIRS         0x002
#define TIMER_B_TGCR_TIMMODE_UD32    0x004
#define TIMER_B_TGCR_TIMMODE_WDT64   0x008
#define TIMER_B_TGCR_TIMMODE_CD32    0x00c
#define TIMER_B_TGCR_TIMMODE_MASK    0x00c
#define TIMER_B_TGCR_PSCHI_MASK      (0x00f << 8)
#define TIMER_B_TGCR_TDDRHI_MASK     (0x00f << 12)

#define TIMER_B_WDTCR_WDEN_ENABLE    (0x01 << 14)

#define TIMER_WDTCR_WDKEY_SEQ0       0xa5c6
#define TIMER_WDTCR_WDKEY_SEQ1       0xda7e
#define TIMER_WDTCR_WDKEY_SHIFT      16

#define TIMER_0                      0
#define TIMER_1                      1
#if defined(CONFIG_SOC_TMS320C6472) || defined(CONFIG_SOC_TMS320C6474)
#define TIMER_2                      2
#define TIMER_3                      3
#define TIMER_4                      4
#define TIMER_5                      5
#define TIMER_6                      6
#define TIMER_7                      7
#define TIMER_8                      8
#define TIMER_9                      9
#define TIMER_10                     10
#define TIMER_11                     11
#endif /* defined(CONFIG_SOC_TMS320C6472) || defined(CONFIG_SOC_TMS320C6474) */
#if defined(CONFIG_SOC_TMS320C6670)
#define TIMER_2                      2
#define TIMER_3                      3
#define TIMER_4                      4
#define TIMER_5                      5
#define TIMER_6                      6
#define TIMER_7                      7
#endif /* defined(CONFIG_SOC_TMS320C6670) */
#if defined(CONFIG_SOC_TMS320C6678)
#define TIMER_2                      2
#define TIMER_3                      3
#define TIMER_4                      4
#define TIMER_5                      5
#define TIMER_6                      6
#define TIMER_7                      7
#define TIMER_8			     8
#define TIMER_9                      9
#define TIMER_10                     10
#define TIMER_11                     11
#define TIMER_12              	     12
#define TIMER_13                     13
#define TIMER_14                     14
#define TIMER_15                     15
#endif /* defined(CONFIG_SOC_TMS320C6678) */

/*
 * Timer clocks are divided down from the CPU clock
 * The divisor is in the EMUMGTCLKSPD register
 */
#define TIMER_DIVISOR(t) \
	((__raw_readl(TIMER_EMUMGTCLKSPD_REG(t)) & (0xf << 16)) >> 16)

#define timer_period(f, d)            (((f) * 1000000) / ((d) * HZ))
#define ticks2usecs(f, d, x)          (((x) * (d)) / (f))

#endif
