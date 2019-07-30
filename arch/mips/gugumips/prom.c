#include <linux/of_fdt.h>
#include <asm/prom.h>

#if IS_ENABLED(CONFIG_GUGUMIPS_NSCSCC)
#define EARLY_PRINTK_UART_BASE 0xbfd00000
#endif

void __init prom_putchar(char c)
{
#if IS_ENABLED(CONFIG_GUGUMIPS_NSCSCC)
	while ((readl((uint32_t *)(EARLY_PRINTK_UART_BASE + 0x8))) & 0x8)
		;
	writel(c, (uint32_t *)(EARLY_PRINTK_UART_BASE + 0x4));
#endif
}

void __init prom_init(void)
{
#if IS_ENABLED(CONFIG_GUGUMIPS_NSCSCC)
    // Enable interrupt, clear fifo
	writel(0x13, (uint32_t *)(EARLY_PRINTK_UART_BASE + 0xC));
#endif
}

void __init prom_free_prom_memory(void)
{
}
