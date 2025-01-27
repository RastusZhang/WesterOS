/*
 * Early boot stage. Sanity checks for hardware devices & "calls" kmain(...)
 *
 * Author: Tuna CICI
 */

#include <stdint.h>
#include <stdarg.h>

#include "ARM64/Machine.h"
#include "MemoryLayout.h"

#include "LibKern/Time.h"
#include "LibKern/Console.h"

extern void kmain(void);

extern uint64_t kstart;
extern uint64_t kend;

void _halt(const char *s)
{
        klog("Halting due to: ");
        kprintf(s);
 
        wfi();
}

/* Check if DTB exists */
void _fdt_check(void)
{
        volatile uint32_t *address = (uint32_t*) DTB_START;
        volatile uint32_t *magic = (uint32_t*) 0xEDFE0DD0; /* 0xDOODFEED */

        if (*address == magic) {
                klog("DTB");
        } else {
                _halt("DTB not");
        }

        kprintf(" found at Physical Address: 0x%p\n", &address);
}

void start(void)
{
        volatile uint32_t arch;
        volatile uint32_t val32;
        volatile uint64_t val64;

        /* Hard-coded device/board info */
        /* TODO: Replace this with a DTB parser */
        const char      *_cpuModel  = "Cortex A-72";
        const uint32_t  *_coreCount = (uint32_t*) 0x2;

        /* Symbols. Defined in Kernel/kernel.ld */
        const uint64_t *kernelBase = (uint64_t*) &kstart;
        const uint64_t *kernelEnd = (uint64_t*) &kend;

        klog("WesterOS early boot stage\n");
        klog("Running sanity checks...\n");

        /* TODO: Any better way to early print? */
        val64 = PL011_BASE;
        klog("WARN: Raw printing directly to PL011 @ 0x%x\n", &val64);

        /* -------- CPU -------- */
        klog("Checking CPU\n");
        MRS("MIDR_EL1", val32);
        arch = (val32 & 0xFF000000) >> 24;

        if (arch == 0x41) {
                klog("---- Implementer: ARM\n");
        } else {
                _halt("---- Unknown Implementer\n");
        }

        klog("---- Model: %s\n", _cpuModel);
        klog("---- SMP: %u\n", &_coreCount);

        MRS("CNTFRQ_EL0", val64);
        val64 = val64 / 1000000; /* Hz to MHz */

        klog("---- Running @ %u MHz\n", &val64);

        klog("---- Current exception level: ");
        MRS("CurrentEL", val32);
        val32 = (val32 & 0x0C) >> 2;

        switch(val32) {
                case 0:
                        kprintf("EL0 (User mode)\n");
                break;
                case 1:
                        kprintf("EL1 (Kernel mode)\n");
                break;
                case 2:
                        kprintf("EL2 (Hypervisor mode)\n");
                break;
                case 3:
                        kprintf("EL3 (Secure Monitor mode)\n");
                break;
                default:
                        _halt("Unknown exception level\n");
                break;
        }

        /* Check if interrupts are enabled */
        klog("---- Interrupts: ");
        MRS("DAIF", val32);

        if (val32 & (1 << 7)) {
                kprintf("Enabled\n");
        } else {
                kprintf("Disabled\n");
        }

        /* -------- Memory -------- */
        klog("Checking Memory\n");
        _fdt_check();
        klog("For now I skip DTB and instead hard-code everything :/\n");

        val64 = RAM_SIZE / (1024 * 1024); /* Bytes to MiB */
        klog("Total available RAM area: %u MiB\n", &val64);

        klog("QEMU ARM Virt Machine memory layout:\n");

        val64 = BOOTROM_START;
        klog("---- BootROM Code: 0x%x", &val64);
        val64 = BOOTROM_END;
        kprintf(" - 0x%x (reserved)\n", &val64);

        val64 = GIC_BASE;
        klog ("---- GIC: 0x%x", &val64);
        val64 = GIC_END;
        kprintf (" - 0x%x (controller)\n", &val64);

        val64 = PL011_BASE;
        klog ("---- PL011 UART: 0x%x", &val64);
        val64 = PL011_END;
        kprintf (" - 0x%x (mmio)\n", &val64);

        val64 = PL031_BASE;
        klog ("---- PL031  RTC: 0x%x", &val64);
        val64 = PL031_END;
        kprintf (" - 0x%x (mmio)\n", &val64);

        val64 = PL061_BASE;
        klog ("---- PL061 GPIO: 0x%x", &val64);
        val64 = PL061_END;
        kprintf (" - 0x%x (mmio)\n", &val64);

        val64 = FW_CFG_BASE;
        klog ("---- QEMU fw_cfg: 0x%x", &val64);
        val64 = FW_CFG_END;
        kprintf (" - 0x%x (qemu)\n", &val64);

        val64 = DTB_START;
        klog ("---- Device Tree Blob: 0x%x", &val64);
        val64 = DTB_END;
        kprintf (" - 0x%x (reserved)\n", &val64);

        klog ("---- Kernel: 0x%x", &kernelBase);
        kprintf (" - 0x%x (system)\n", &kernelEnd);

        val64 = RAM_SIZE;
        val64 -= DTB_SIZE;
        val64 -= (kernelEnd - kernelBase);
        val64 /= (1024 * 1024);
        klog("Total usable RAM area: %u MiB\n", &val64);

        klog("Everything's OK. Calling the Kernel now...\n");
        kmain();
}
