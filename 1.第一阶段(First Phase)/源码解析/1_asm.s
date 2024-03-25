/*该文件控制中断前的处理过程和中断后的恢复过程，
其中中断的处理过程有traps.c控制*/
/*控制形式为硬中断（硬件中断的处理过程）*/
# 一下是函数声明，具体作用在traps.c中有所体现
.globl _divide_error,_debug,_nmi,_int3,_overflow,_bounds,_invalid_op
.globl _double_fault,_coprocessor_segment_overrun
.globl _invalid_TSS,_segment_not_present,_stack_segment
.globl _general_protection,_coprocessor_error,_irq13,_reserved

# pushl为入栈过程
# popl为出栈过程
_divide_error:
	pushl $_do_divide_error		# 将traps.c中的函数do_divide_error压入栈中
no_error_code:					# 无错误码
	# esp为堆栈指针，eax为通用寄存器,即将刚入栈的do_divide_error函数的地址传入寄存器eax中
	xchgl %eax,(%esp)			# 此时eax为C语言函数入口地址
	pushl %ebx					# 将ebx压入栈
	pushl %ecx					# 将ecx压入栈
	pushl %edx					# 将edx压入栈
	pushl %edi					# 将edi压入栈
	pushl %esi					# 将esi压入栈
	pushl %ebp					# 将ebp压入栈
	push %ds 					# 将ds压入栈
	push %es  					# 将es压入栈
	push %fs 					# 将fs压入栈
	pushl $0					# 无错误码，所以将0压入栈中
	# eax,ebx,ecx等皆为寄存器
	# push和pushl都是入栈指令，pushl定义入栈的类型为long，即双字（两个字节或四个字节）
	lea 44(%esp),%edx			# 将esp的内容加上十进制的44，存入edx中
	pushl %edx					# 将新存内容后的edx寄存器压入栈中
	movl $0x10,%edx				# 将0x10赋值给edx
	mov %dx,%ds 				# 将dx中数值赋值给ds
	mov %dx,%es 				# 将dx中数值赋值给es
	mov %dx,%fs 				# 将dx中数值赋值给fs
	call *%eax					# 调用在代码首部压入eax寄存器中C函数
	addl $8,%esp 				# 给先在esp堆栈指针加8,此时esp指向压栈时fs寄存器的位置
	pop %fs						# 将fs弹出栈
	pop %es      				# 将es弹出栈
	pop %ds 					# 将ds弹出栈
	popl %ebp					# 将ebp弹出栈
	popl %esi					# 将esi弹出栈
	popl %edi					# 将edi弹出栈
	popl %edx					# 将edx弹出栈
	popl %ecx					# 将ecx弹出栈
	popl %ebx					# 将ebx弹出栈
	popl %eax					# 将eax弹出栈
	# popl和pushl相对应，即对应位数须保持一致
	iret						# 恢复中断的地址

_debug:							# debug调试中断
	pushl $_do_int3				# 将do_int3压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_nmi:							# nmi非屏蔽中断(no mark interrupt)
	pushl $_do_nmi				# 将do_nmi压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_int3:							# 断点指令中断
	pushl $_do_int3				# 将do_int3压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_overflow:						# 溢出错误中断
	pushl $_do_overflow			# 将do_overflow压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_bounds:						# 边境监察点错误中断
	pushl $_do_bounds			# 将do_bounds压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_invalid_op:					# 无效操作指令中断			
	pushl $_do_invalid_op		# 将do_invalid_op压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_coprocessor_segment_overrun:	# 协处理器超出中断
	pushl $_do_coprocessor_segment_overrun	# 将do_coprocessor_segment_onerrun压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_reserved:						# 保留其他中断
	pushl $_do_reserved			# 将do_redervedr压入栈中，即调用
	jmp no_error_code			# 跳转到无错误码的情况

_irq13:							# 发出IRQ13中断信号
	pushl %eax					# 将eax压入栈
	xorb %al,%al 				# 对两个寄存器数据进行按位异或
	outb %al,$0xF0				# 将0xF0写入al寄存器中
	movb $0x20,%al 				# 将0x20赋值给寄存器al
	outb %al,$0x20				# 将0x20寄存器写入al寄存器中
	jmp 1f						# 跳转至1
1:	jmp 1f						# 跳转至1
1:	outb %al,$0xA0				# 将0xA0写入al寄存器中
	popl %eax					# 将寄存器eax弹出栈
	jmp _coprocessor_error		# 跳转到traps.c中的函数

_double_fault:					# 双出错中断
	pushl $_do_double_fault		# 调用do_double_fault函数
error_code:						# 有错误码
	xchgl %eax,4(%esp)			# 将esp所指栈位加4后与eax交换
	xchgl %ebx,(%esp)			# 将esp所指位置与ebx交换
	pushl %ecx					# 将ecx压入栈
	pushl %edx					# 将edx压入栈
	pushl %edi					# 将edi压入栈
	pushl %esi					# 将esi压入栈
	pushl %ebp					# 将ebp压入栈
	push %ds 					# 将ds压入栈
	push %es 					# 将es压入栈
	push %fs					# 将fs压入栈
	pushl %eax					# 将eax压入栈
	lea 44(%esp),%eax			# 将esp的内容加上十进制的44，存入eax中
	pushl %eax					# 将eax压入栈
	movl $0x10,%eax				# 将0x10赋值给eax
	mov %ax,%ds 				# 将寄存器ax中的值赋值给ds
	mov %ax,%es 				# 将寄存器ax中的值赋值给es
	mov %ax,%fs					# 将寄存器ax中的值赋值给fs
	call *%ebx					# 调用压入ebx寄存器中的函数
	addl $8,%esp 				# 给先在esp堆栈指针加8,此时esp指向压栈时fs寄存器的位置
	pop %fs						# 将fs弹出栈
	pop %es 					# 将es弹出栈
	pop %ds 					# 将ds弹出栈
	popl %ebp					# 将ebp弹出栈
	popl %esi					# 将esi弹出栈
	popl %edi					# 将edi弹出栈
	popl %edx					# 将edx弹出栈
	popl %ecx					# 将ecx弹出栈
	popl %ebx					# 将ebx弹出栈
	popl %eax					# 将eax弹出栈
	iret						# 恢复中断的地址

_invalid_TSS:				# 无效任务状态段后产生中断
	pushl $_do_invalid_TSS	# 将do_invalid_TSS压入栈中，即调用该函数
	jmp error_code			# 跳转到有错误码的情况

_segment_not_present:		# 段不存在出现中断
	pushl $_do_segment_not_present	# 将do_segment_not_present压入栈中，即调用该函数
	jmp error_code 			# 跳转到有错误码的情况

_stack_segment:				# 堆栈段错误产生中断
	pushl $_do_stack_segment	# 将do_stack_segment压入栈中，即调用该函数
	jmp error_code 			# 跳转到有错误码的情况

_general_protection:		# 保护性出错产生中断
	pushl $_do_general_protection	# 将do_general_protection压入栈中，即调用该函数
	jmp error_code 			# 跳转到有错误码的情况