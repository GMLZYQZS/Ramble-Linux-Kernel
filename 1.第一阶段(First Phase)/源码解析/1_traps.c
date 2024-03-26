/*中断时的执行过程
其中文件控制中断前的处理过程和中断后的恢复过程由asm.s控制*/
/*控制形式为硬中断（硬件中断的处理过程）*/
#include <string.h>

#include <linux/head.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/system.h>
#include <asm/segment.h>
#include <asm/io.h>
/*以上为头文件
分别为：<string.h>：字符串头文件，主要定义了一些有关字符串操作的嵌入函数。
<linux/head.h>：head头文件，定义了段描述符的简单结构，和几个选择符常量。
<linux/sched.h>： 调度程序头文件，定义了任务结构task_struct、初始任务0的数据，以及获取汇编函数语句。
<linux/kernel.h>：内核头文件，提供常用函数，例如：printk
<asm/system.h>：系统头文件，定义了设置或修改描述符/中断门等的嵌入式汇编宏。
<asm/segment.h>：段操作头文件，定义了有关段寄存器操作的嵌入式汇编函数。
<asm/io.h>：I/O头文件，以宏的嵌入汇编程序形式定义对I/O端口操作的函数。*/

//定义全局三个嵌入式汇编函数
#define get_seg_byte(seg,addr) ({ \					//取seg段中addr处的一个字节
register char __res; \								//定义了一个寄存器变量_res,存储类型为char
__asm__("push %%fs;mov %%ax,%%fs;movb %%fs:%2,%%al;pop %%fs" \
//将段fs寄存器内容入栈，将eax寄存器的值赋值给段fs，取seg:addr处1字节内容到寄存器al，将fs段寄存器弹出栈
	:"=a" (__res):"0" (seg),"m" (*(addr))); \
//输出寄存器：eax，将运行后的结果放入_res;输入寄存器：eax，代码开始运行时将seg放入eax
__res;})										//_res接收该宏函数的值

#define get_seg_long(seg,addr) ({ \					//取seg段中addr处的四个字节
register unsigned long __res; \						//定义了一个寄存器变量_res,存储类型为unsigned long
__asm__("push %%fs;mov %%ax,%%fs;movl %%fs:%2,%%eax;pop %%fs" \	
//将段fs寄存器内容入栈，将eax寄存器的值赋值给段fs，取seg:addr处1字节内容到寄存器eax，将fs段寄存器弹出栈
	:"=a" (__res):"0" (seg),"m" (*(addr))); \
//输出寄存器：eax，将运行后的结果放入_res;输入寄存器：eax，代码开始运行时将seg放入eax
__res;})										//_res接收该函数的值

#define _fs() ({ \									//取段fs寄存器中的值
register unsigned short __res; \					//定义了一个寄存器变量_res,存储类型为unsigned short
__asm__("mov %%fs,%%ax":"=a" (__res):); \			//输出寄存器：eax;输入：无
__res;})											//_res接收该函数的值

//此处声明的函数与asm.s中所调用的C函数一一对应
int do_exit(long code);			//该函数函数名为do_exit,返回值为int类型，调用参数为long类型

void page_exception(void);		//该函数函数名为do_exit,返回值为int类型，调用参数为long类型
//()位置保存的为处理后的代码位置,例如：(kernel/asm.s-13)表示在kernel文件夹里的asm.s文件的13行
void divide_error(void);		//该函数函数名为divide_error,返回值为空类型，调用参数为空类型(kernel/asm.s-13)
void debug(void);				//该函数函数名为debug,返回值为空类型，调用参数为空类型(kernel/asm.s-51)
void nmi(void);					//该函数函数名为nmi,返回值为空类型，调用参数为空类型(kernel/asm.s-55)
void int3(void);				//该函数函数名为int3,返回值为空类型，调用参数为空类型(kernel/asm.s-59)
void overflow(void);			//该函数函数名为overflow,返回值为空类型，调用参数为空类型(kernel/asm.s-63)
void bounds(void);				//该函数函数名为bounds,返回值为空类型，调用参数为空类型(kernel/asm.s-67)
void invalid_op(void);			//该函数函数名为invalid_op,返回值为空类型，调用参数为空类型(kernel/asm.s-71)
void device_not_available(void);//该函数函数名为device_not_available,返回值为空类型，调用参数为空类型(kernel/sys_call.s-158)
void double_fault(void);		//该函数函数名为double_fault,返回值为空类型，调用参数为空类型(kernel/asm.s-95)
void coprocessor_segment_overrun(void);	//该函数函数名为coprocessor_segment_overrun,返回值为空类型，调用参数为空类型(kernel/asm.s-75)
void invalid_TSS(void);			//该函数函数名为invalid_TSS,返回值为空类型，调用参数为空类型(kernel/asm.s-129)
void segment_not_present(void);	//该函数函数名为segment_not_present,返回值为空类型，调用参数为空类型
void stack_segment(void);		//该函数函数名为stack_segment,返回值为空类型，调用参数为空类型
void general_protection(void);	//该函数函数名为general_protection,返回值为空类型，调用参数为空类型
void page_fault(void);			//该函数函数名为page_fault,返回值为空类型，调用参数为空类型
void coprocessor_error(void);	//该函数函数名为coprocessor_error,返回值为空类型，调用参数为空类型
void reserved(void);			//该函数函数名为reserved,返回值为空类型，调用参数为空类型
void parallel_interrupt(void);	//该函数函数名为parallel_interrupt,返回值为空类型，调用参数为空类型
void irq13(void);				//该函数函数名为irq13,返回值为空类型，调用参数为空类型

//char * str定义一个字符串指针，该指针指向字符串的第一位，可用来代表一个字符串
//long eap_ptr 定义长整型的栈指针
//long 定义长整型的段号
static void die(char * str,long esp_ptr,long nr)		//定义了一个静态的空返回值的die函数
{
	long * esp = (long *) esp_ptr;						//定义长类型指针eap，将esp_ptr强制转换为长指针类型
	int i;												//定义整形变量i

	printk("%s: %04x\n\r",str,nr&0xffff);				//打印str(str中为中断类型)和nr&0xffff即错误码
	printk("EIP:\t%04x:%p\nEFLAGS:\t%p\nESP:\t%04x:%p\n",	
		esp[1],esp[0],esp[2],esp[4],esp[3]);			//打印EIP,EFLAGS,ESP段寄存器的值
	printk("fs: %04x\n",_fs());							//打印FS段寄存器值
	printk("base: %p, limit: %p\n",get_base(current->ldt[1]),get_limit(0x17));	
	//get_base()与get_limit()函数皆在include/linux/sched.h定义;current->ldt[1]表示当前任务代码段，0x17选择符指向特定的ldt位置
	if (esp[4] == 0x17) {					//如果eap[4]此时代表的寄存器为ss为0x17(用户栈)
		printk("Stack: ");					//则打印出用户栈中的内容
		for (i=0;i<4;i++)					//初值i=1,如果i<4执行循环体，循环体执行完毕后i++
			printk("%p ",get_seg_long(0x17,i+(long *)esp[3]));		//打印段指针指向的esp[0]-esp[3]寄存器内容与地址
		printk("\n");									//打印换行
	}
	str(i);									//取当前任务的任务号，该函数定义在include/linux/sched.h
	printk("Pid: %d, process nr: %d\n\r",current->pid,0xffff & i);	//PID即进程号，Process nr即任务号
	for(i=0;i<10;i++)						//初值i=1,如果i<10执行循环体，循环体执行完毕后i++
		printk("%02x ",0xff & get_seg_byte(esp[1],(i+(char *)esp[0])));	//打印段指针指向的esp[1]-esp[9]寄存器内容与地址
	printk("\n\r");														//打印换行
	do_exit(11);														//do_exit函数在kernel/exit.c中定义，返回警告
}

void do_double_fault(long esp, long error_code)		//定义函数名为double_fault,返回值为空类型，调用参数为双long型
{
	die("double fault",esp,error_code);				//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_general_protection(long esp, long error_code)	//定义函数名为do_general_protection,返回值为空类型，调用参数为双long型
{
	die("general protection",esp,error_code);		//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_divide_error(long esp, long error_code)		//定义函数名为do_divide_error,返回值为空类型，调用参数为双long型
{
	die("divide error",esp,error_code);				//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_int3(long * esp, long error_code,			//定义函数名为do_int3,返回值为空类型，调用参数分别为：
		long fs,long es,long ds,					//一个指针类型，和11个long类型
		long ebp,long esi,long edi,					//该函数调用的参数与asm.s函数对应
		long edx,long ecx,long ebx,long eax)		//分别为段寄存器esp和剩下11个寄存器(kernel/asm.s_16-30)
{
	int tr;											//定义整形tr

	__asm__("str %%ax":"=a" (tr):"0" (0));			//该汇编函数取任务寄存器值
	printk("eax\t\tebx\t\tecx\t\tedx\n\r%8x\t%8x\t%8x\t%8x\n\r",		//输出八个字符长的16进制
		eax,ebx,ecx,edx);												//打印eax,ebx,ecx.edx值
	printk("esi\t\tedi\t\tebp\t\tesp\n\r%8x\t%8x\t%8x\t%8x\n\r",		//输出八个字符长的16进制
		esi,edi,ebp,(long) esp);										//打印esi,edi,ebp以及长类型的esp值
	printk("\n\rds\tes\tfs\ttr\n\r%4x\t%4x\t%4x\t%4x\n\r",				//输出四个字符长的16进制
		ds,es,fs,tr);													//打印ds,es,fs,tr值
	printk("EIP: %8x   CS: %4x  EFLAGS: %8x\n\r",esp[0],esp[1],esp[2]);	//打印esp[0]-esp[2]值
}																		//对应的分别为EIP,CS,EFLAGS

void do_nmi(long esp, long error_code)			//定义函数名为do_nmi,返回值为空类型，调用参数为双long型
{
	die("nmi",esp,error_code);					//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_debug(long esp, long error_code)		//定义函数名为do_debug,返回值为空类型，调用参数为双long型
{
	die("debug",esp,error_code);				//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_overflow(long esp, long error_code)		//定义函数名为do_overflow,返回值为空类型，调用参数为双long型
{
	die("overflow",esp,error_code);				//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_bounds(long esp, long error_code)		//定义函数名为do_bounds,返回值为空类型，调用参数为双long型
{
	die("bounds",esp,error_code);				//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_invalid_op(long esp, long error_code)	//定义函数名为do_invalid_op,返回值为空类型，调用参数为双long型
{
	die("invalid operand",esp,error_code);		//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_device_not_available(long esp, long error_code)	//定义函数名为do_device_not_available,返回值为空类型，调用参数为双long型
{
	die("device not available",esp,error_code);			//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_coprocessor_segment_overrun(long esp, long error_code)	//定义函数名为do_coprocessor_segment_overrun,返回值为空类型，调用参数为双long型
{
	die("coprocessor segment overrun",esp,error_code);			//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_invalid_TSS(long esp,long error_code)	//定义函数名为do_invalid_TSS,返回值为空类型，调用参数为双long型
{
	die("invalid TSS",esp,error_code);			//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_segment_not_present(long esp,long error_code)	//定义函数名为do_segment_not_present,返回值为空类型，调用参数为双long型
{
	die("segment not present",esp,error_code);			//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_stack_segment(long esp,long error_code)			//定义函数名为do_stack_segment,返回值为空类型，调用参数为双long型
{
	die("stack segment",esp,error_code);				//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_coprocessor_error(long esp, long error_code)	//定义函数名为do_coprocessor_error,返回值为空类型，调用参数为双long型
{
	if (last_task_used_math != current)					//如果最后一个任务！=（不是）当前任务
		return;											//返回
	die("coprocessor error",esp,error_code);			//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void do_reserved(long esp, long error_code)			//定义函数名为do_reserved,返回值为空类型，调用参数为双long型
{
	die("reserved (15,17-47) error",esp,error_code);//调用die函数，将字符串、段寄存器地址、错误码传入函数
}

void trap_init(void)								//中断初始化函数
{													//该函数对应的中断与asm.s的中断对应且相同
	int i;											//定义整型i
													//中断向量是CPU用来响应硬件设备请求服务的中断信号索引
	set_trap_gate(0,&divide_error);					//设置divide_error中断向量值为0（除数为零是中断）
	set_trap_gate(1,&debug);						//设置debug中断向量值为1（调试中断）
	set_trap_gate(2,&nmi);							//设置nmi中断向量值为2（非屏蔽中断）
	set_system_gate(3,&int3);						//设置int3中断向量值为3（断点指令中断）
	set_system_gate(4,&overflow);					//设置overflow中断向量值为4（溢出中断）
	set_system_gate(5,&bounds);						//设置bounds中断向量值为5（寻址有误中断）
	set_trap_gate(6,&invalid_op);					//设置invalid_op中断向量值为6（操作码有误中断）
	set_trap_gate(7,&device_not_available);			//设置device_not_available中断向量值为7（设备不存在中断）
	set_trap_gate(8,&double_fault);					//设置double_fault中断向量值为8（双故障中断）
	set_trap_gate(9,&coprocessor_segment_overrun);	//设置coprocessor_segment_overrun中断向量值为9（段超出中断）
	set_trap_gate(10,&invalid_TSS);					//设置invalid_TSS中断向量值为10（TSS无效中断）
	set_trap_gate(11,&segment_not_present);			//设置segment_not_present中断向量值为11（描述段错误中断）
	set_trap_gate(12,&stack_segment);				//设置stack_segment中断向量值为12（栈有误中断）
	set_trap_gate(13,&general_protection);			//设置general_prtection中断向量值为13（保护机制中断）
	set_trap_gate(14,&page_fault);					//设置page_fault中断向量值为14（内存页缺失中断）
	set_trap_gate(15,&reserved);					//中断入口
	set_trap_gate(16,&coprocessor_error);			//设置coprocessor_error中断向量值为16（信号错误中断）
	for (i=17;i<48;i++)								//初始值i=17，i<48时执行循环体，循环体结束后i++
		set_trap_gate(i,&reserved);					//将17--47均设置为reserved（待定）
	set_trap_gate(45,&irq13);						//设置irq13中断向量值为45（中断请求）
	outb_p(inb_p(0x21)&0xfb,0x21);					//允许8259A主芯片的IRQ2中断请求
	outb(inb_p(0xA1)&0xdf,0xA1);					//允许8259A从芯片的IRQ2中断请求。
	set_trap_gate(39,&parallel_interrupt);			//设置parallel_interrupt中断向量值为39（设置并行口中断）
}													//涉及优先级问题，在内存部分详说
