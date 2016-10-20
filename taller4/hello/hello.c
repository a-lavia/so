#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int hello_init(void) {
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
	return 0;
}


static void hello_exit(void) {
	printk(KERN_ALERT "Adios, mundo cruel...\n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Juan de los Palotes");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");