#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <plat/gpio-cfg.h>
#include <asm/irq.h>
#include <mach/gpio.h>

#define DEVICE_NAME     "led"
#define NUM_LEDS         1
struct led_dev {
	dev_t led_number;
	struct cdev cdev;
	struct device *devicep;
	struct class *classp;
} *led;


/**  _IO (魔数，基数);
 * 魔数 (magic number):
 * 	魔数范围为 0~255。通常，用英文字符 "A" ~ "Z"或者 "a" ~ "z"来表示。设备驱动程
 * 序从传递进来的命令获取魔数，然后与自身处理的魔数想比较，如果相同则处理，不同则不处理。
 * 魔数是拒绝误使用的初步辅助状态。设备驱动程序可以通过 _IOC_TYPE (cmd)来获取魔数。
 * 不同的设备驱动程序最好设置不同的魔数，但并不是要求绝对，也是可以使用其他设备驱动程序
 * 已用过的魔数。
 * 基(序列号)数:
 * 	基数用于区别各种命令。通常，从0开始递增，相同设备驱动程序上可以重复使用该值。例如，
 * 读取和写入命令中使用了相同的基数，设备驱动程序也能分辨出来，原因在于设备驱动程序区分
 * 令时使用 switch，且直接使用命令变量 cmd值。创建命令的宏生成的值由多个域组合而成，
 * 所以即使是相同的基数，也会判断为不同的命令。设备驱动程序想要从命令中获取该基数，就使
 * 用下面的宏：
 * _IOC_NR (cmd)
 * 通常，switch中的 case值使用的是命令的本身。
 * */
#define LED_PIN    		EXYNOS4212_GPM1(4)
#define LED_IOC_MAGIC          'L'
#define LED_ON			_IO(LED_IOC_MAGIC, 0)
#define LED_OFF			_IO(LED_IOC_MAGIC, 1)

static long uptech_leds_ioctl(
        struct file *file,
        unsigned int cmd,
        unsigned long arg)
{
	printk("cmd = %d\n", cmd);
	gpio_request(LED_PIN, "LED_PIN");

        switch(cmd) {
        case LED_ON:
		gpio_direction_output(LED_PIN, 1);
		break;
        case LED_OFF:
		gpio_direction_output(LED_PIN, 0);
		break;
        default:
		goto err;
        }

	gpio_free(LED_PIN);
	
	return 0;
err:
	gpio_free(LED_PIN);
        return -EINVAL;
}

/** 在kernel 2.6.36 中已经完全删除了struct file_operations 中的ioctl 函数指针，
 * 取而代之的是unlocked_ioctl  在应用程序中ioctl是兼容的，不必变化。
 * 而在驱动程序中这个指针函数变了之后最大的影响是参数中少了inode ，
 * 所以应用程序ioctl是兼容的，但驱动程序中我们的ioctl函数必须变化,
 * 否则就会发生cmd参数的变化
 * */
static struct file_operations uptech_leds_fops = {
        .owner  =       THIS_MODULE,
        .unlocked_ioctl  =       uptech_leds_ioctl,
};
 
 /** 宏定义__init,用于告诉编译器相关函数或变量仅用于初始化。编译器将标__init的所有代
  * 码存在特殊的内存段中，初始化结束后就释放这段内存。
  * 如：static int __init uptech_leds_init(void);
  **/
static int __init uptech_leds_init(void)
{
        int r;
        
/** kzalloc() 函数与 kmalloc() 非常相似，参数及返回值是一样的，可以说是前者>是后者
 * 的一个变种，因为 kzalloc() 实际上只是额外附加了 __GFP_ZERO 标志。所以它除>了申
 * 请内核内存外，还会对申请到的内存内容清零。
 * kzalloc - allocate memory. The memory is set to zero.
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate (see kmalloc).
 * static inline void *kzalloc(size_t size, gfp_t flags){    
 * return kmalloc(size, flags | __GFP_ZERO);}
 * kzalloc() 对应的内存释放函数也是 kfree()。
 **/
	led = kzalloc(sizeof(struct led_dev), GFP_KERNEL);
	
	/** IS_ERR宏定义在include/linux/err.h
	 * #define IS_ERR_VALUE(x) unlikely((x) > (unsigned long)-1000L)
	 * static inline void *ERR_PTR(long error){
	 *    return (void *) error;
	 * }
	 * static inline long PTR_ERR(const void *ptr){
	 *    return (long) ptr;
	 * }
	 * static inline long IS_ERR(const void *ptr){
	 *    return IS_ERR_VALUE((unsigned long)ptr);
	 * }
	 * 
	 * /include/linux/compiler.h中，具体定义如下：
	 * #define likely(x) __builtin_expect(!!(x), 1)
	 * #define unlikely(x) __builtin_expect(!!(x), 0)
	 * 注解为：
	 * You may use __builtin_expect to provide the compiler with branch 
	 * prediction information. In general, you should prefer to use 
	 * actual profile feedback for this (‘-fprofile-arcs’), as 
	 * programmers are notoriously bad at predicting how their programs 
	 * actually perform. However, there are applications in which this 
	 * data is hard to collect.The return value is the value of exp, 
	 * which should be an integral expression. The semantics of the 
	 * built-in are that it is expected that exp == c.
	 * 人为告诉编译器那种分支的几率更大一些。以此提高与去指令的正确率，因而可提高效率。
	 **/	
	if (IS_ERR(led)) {
		r = PTR_ERR(led);
		pr_err("kmalloc() failed %d\n", r);
		goto err1;
	}

	// class_create动态创建设备的逻辑类，并完成部分字段的初始化，然后将其添加到内核中。创建的逻辑类位于/sys/class/。
	// 参数：
	//  owner, 拥有者。一般赋值为THIS_MODULE。
	//  name, 创建的逻辑类的名称。
        led->classp = class_create(THIS_MODULE, DEVICE_NAME);
        if(IS_ERR(led->classp)) {
		r = PTR_ERR(led->classp);
		pr_err("class_create() failed %d\n", r);
		goto err2;
        }
        
/** 内核提供了三个函数来注册一组字符设备编号，这三个函数分别是 
 * register_chrdev_region()、alloc_chrdev_region() 和 register_chrdev()。
 * 这三个函数都会调用一个共用的 __register_chrdev_region() 函数来注册一组设备编号
 * 范围（即一个 char_device_struct 结构）。
 * register_chrdev_region(dev_t first,unsigned int count,char *name) 
 * First :要分配的设备编号范围的初始值(次设备号常设为0); 
 * Count:连续编号范围. 
 * Name:编号相关联的设备名称. (/proc/devices); 
 * 动态分配: 
 * Int alloc_chrdev_region(dev_t *dev,unsigned int firstminor,unsigned int count,char *name); 
 * Firstminor : 通常为0; 
 * dev:存放返回的设备号; 
 * 释放: 
 * Void unregist_chrdev_region(dev_t first,unsigned int count); 
 * 调用Documentation/devices.txt中能够找到已分配的设备号．
 * */
	r = alloc_chrdev_region(&led->led_number, 0, 
					NUM_LEDS, DEVICE_NAME);
	if (r) {
		pr_err("alloc_chrdev_region() failed %d\n", r);
		goto err3;
	}

/** <include/linux/cdev.h>
 * struct cdev {
 *     struct kobject kobj;    //内嵌的内核对象.
 *     struct module *owner;   //该字符设备所在的内核模块的对象指针.
 *     const struct file_operations *ops;    //该结构描述了字符设备所能实现的方法，
 * 是极为关键的一个结构体.
 *     struct list_head list;  //用来将已经向内核注册的所有字符设备形成链表.
 *     dev_t dev;              //字符设备的设备号，由主设备号和次设备号构成.
 *     unsigned int count;     //隶属于同一主设备号的次设备号的个数.
 * };
 * */
	cdev_init(&led->cdev, &uptech_leds_fops);
	led->cdev.owner = THIS_MODULE;
	/** int cdev_add(struct cdev *p, dev_t dev, unsigned count)
	 * 我们之前获得设备号和设备号长度填充到cdev结构中
	 * 封装好一个probe结构并将它的地址放入probes数组进而封装进cdev_map,根据传入的
	 * 设备号的个数，将设备号和cdev依次封装到kmalloc_array分配的n个probe结构中，
	 * 遍历probs数组，直到找到一个值为NULL的元素，再将probe的地址存入probes。至此，
	 * 我们就将我们的cdev放入的内核的数据结构，当然，cdev中大量属性都是由内核帮我们填
	 * 充的。
	 * */
    r = cdev_add(&led->cdev, led->led_number, 1);
	if (r) {
		pr_err("cdev_add() failed %d\n", r);
		goto err4;
	}
/** struct device *device_create(struct class *class, 
 * struct device *parent,dev_t devt, void *drvdata,const char *fmt,...)
 * @class: pointer to the struct class that this device should be registered to
 * 	the struct class 指针，必须在本函数调用之前先被创建
 * @parent: pointer to the parent struct device of this new device
 * 	该设备的parent指针。
 * @devt: the dev_t for the char device to be added
 * 	字符设备的设备号，如果dev_t不是0,0的话，1个”dev”文件将被创建。
 * @drvdata: the data to be added to the device for callbacks
 * 	被添加到该设备回调的数据。
 * @fmt: string for the device's name
 * 	设备名字。
 * */
    led->devicep = device_create(led->classp, NULL, 
		led->led_number, NULL, DEVICE_NAME); 
	if (IS_ERR(led->devicep)) {
		r = PTR_ERR(led->devicep);
		pr_err("device_create() failed %d\n", r);
		goto err5;
	}
 /** int gpio_request(unsigned gpio, const char *label)
  * @gpio:为你要申请的哪一个管脚
  * @label:则是为其取一个名字。
  * */
	gpio_request(LED_PIN, "LED_PIN");
	/** int gpio_direction_output(unsigned gpio, int value)；
	 * 将 GPIO port（由参数 gpio 指定）设为输出，并指定输出电平值（value）
	 * */
	gpio_direction_output(LED_PIN, 0);
	/** void gpio_free(unsigned gpio)；
	 * 释放 GPIO port 的使用权，由参数 gpio 指定具体 port。
	 * */
	gpio_free(LED_PIN);

        printk(DEVICE_NAME "leds initialized\n");

        return 0;
err5:
	cdev_del(&led->cdev);
err4:
	unregister_chrdev_region(MAJOR(led->led_number), NUM_LEDS);
err3:
	class_destroy(led->classp);
err2:
	kfree(led);
err1:
	return r;	
}
 
static void __exit uptech_leds_exit(void)
{
        device_destroy(led->classp, led->led_number);
	unregister_chrdev_region(MAJOR(led->led_number), NUM_LEDS);
        cdev_del(&led->cdev);
        class_destroy(led->classp);
	kfree(led);
}

module_init(uptech_leds_init);
module_exit(uptech_leds_exit);

MODULE_AUTHOR("www.uptech.com");
MODULE_DESCRIPTION("UP-CUP4412 LED Drivers");
MODULE_LICENSE("GPL"); 
