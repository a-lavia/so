#include "fakeserialhelper.h"
#include <linux/kfifo.h>
#include <linux/semaphore.h>

#define DEVICE_NAME "fakeserial"
#define FAKESERIAL_FIFO_SIZE PAGE_SIZE

struct kfifo fifo;
struct semaphore sem_b;
struct semaphore sem_fifo;


void irq_handler(void){
	u8 b = fake_inb(FAKE_SERIAL_PORT);
	kfifo_in(&fifo, &b, sizeof(u8));
	up(&sem_b);
}


static ssize_t read_handler(struct file* file, char __user* buf,
							   size_t count, loff_t *ppos)
{
	u8 b;
	size_t read = 0;
	while(read <= count){
		down_interruptible(&sem_b);
		down_interruptible(&sem_fifo);
		kfifo_out(&fifo, &b, 1);
		up(&sem_fifo);
		copy_to_user(buf, &b, 1);
		read++;
	}

	return count;
}

static const struct file_operations fakeserial_fops = {
	.owner = THIS_MODULE,
	.read = &read_handler,
};

static struct miscdevice fakeserial_dev = {
	MISC_DYNAMIC_MINOR,
	DEVICE_NAME,
	&fakeserial_fops
};


static int __init fakeserial_init(void) {
	int ret;

	printk(KERN_DEBUG "fakeserial: Initialized!\n");

	// Inicializo la pila del kernel
	kfifo_alloc(&fifo, PAGE_SIZE, GFP_KERNEL);

	// Inicializo los semaforos
	sema_init(&sem_b, 0);
	sema_init(&sem_fifo, 1);

	// Inicializo el fake serial
	fake_serial_init(FAKE_SERIAL_MODE_TEXT, irq_handler);

	ret = misc_register(&fakeserial_dev);
	if(ret)
		printk(KERN_ERR "Unable to register \"Fakedriver\" misc device\n");

	return ret;
}

static void __exit fakeserial_exit(void) {
	// Antes de que el módulo sea descargado
	fake_serial_exit();
	
	misc_deregister(&fakeserial_dev);
}

module_init(fakeserial_init);
module_exit(fakeserial_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alumno");
MODULE_DESCRIPTION("Driver del dispositivo serial ficticio");