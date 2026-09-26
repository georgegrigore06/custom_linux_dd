#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xdee352ff, "__platform_driver_register" },
	{ 0x8f7ad95f, "_dev_info" },
	{ 0x14d6dbbb, "devm_kmalloc" },
	{ 0x934d6296, "devm_gpiod_get_index" },
	{ 0x3df967b6, "_dev_err" },
	{ 0x07a93f99, "gpiod_get_value" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0xe81c9efa, "gpiod_to_irq" },
	{ 0x24a3589b, "devm_request_threaded_irq" },
	{ 0xdbd4b560, "platform_driver_unregister" },
	{ 0xbf61f840, "gpiod_set_value" },
	{ 0x92997ed8, "_printk" },
	{ 0x91d66ee9, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Ccustom,mydriver");
MODULE_ALIAS("of:N*T*Ccustom,mydriverC*");

MODULE_INFO(srcversion, "4615C69608E79DEBBFAC0CB");
