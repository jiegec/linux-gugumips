#include <linux/of_fdt.h>
#include <linux/clk-provider.h>
#include <linux/irqchip.h>
#include <linux/clk.h>
#include <linux/clocksource.h>
#include <asm/prom.h>
#include <asm/irq_cpu.h>
#include <asm/time.h>
#include <asm/mips-cps.h>

void __init plat_mem_setup(void)
{
    strlcpy(arcs_cmdline, boot_command_line, COMMAND_LINE_SIZE);
    __dt_setup_arch(__dtb_start);
}

void __init device_tree_init(void)
{
    if (!initial_boot_params)
        return;

    unflatten_and_copy_device_tree();
}

void __init plat_time_init(void) {
    struct device_node *np;
    struct clk *clk;
    of_clk_init(NULL);
    np = of_get_cpu_node(0, NULL);
    if (!np) {
        pr_err("Failed to get CPU node\n");
        return;
    }
    clk = of_clk_get(np, 0);
    if (IS_ERR(clk)) {
        pr_err("Failed to get CPU clock: %ld\n", PTR_ERR(clk));
        return;
    }

    mips_hpt_frequency = clk_get_rate(clk);
    clk_put(clk);
    timer_probe();
}

void __init arch_init_irq(void) {
    unsigned int sr;
    struct device_node *intc_node;

    sr = set_c0_status(ST0_BEV);
    write_c0_ebase((u32)ebase);
    write_c0_status(sr);

    intc_node = of_find_compatible_node(NULL, NULL,
                        "mti,cpu-interrupt-controller");
    of_node_put(intc_node);

    irqchip_init();
}

const char *get_system_type(void)
{
    const char *str;
    int err;

    err = of_property_read_string_index(of_root, "compatible", 0, &str);
    if (!err)
        return str;

    return "Unknown";
}

unsigned int get_c0_compare_int(void) {
    return CP0_LEGACY_COMPARE_IRQ;
}