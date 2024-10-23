/* GPLv2 (c) Airbus */
#include <debug.h>
#include <intr.h>

void bp_handler() {
	asm volatile ("pusha");
	uint32_t val;
   	asm volatile ("mov 4(%%ebp), %0":"=r"(val));
	debug("%x\n",val);

   	debug("WOW BP BP\n");
	asm volatile ("pusha");
	asm volatile ("leave");
	asm volatile ("iret");
}

void bp_trigger() {
	
	asm volatile ("int3");
}

void tp() {
	// TODO print idtr
	unsigned int IDT_add;
	get_idtr(IDT_add);
	debug("IDT Add 0x%x\n",IDT_add);

	idt_reg_t idt_ptr;
	get_idtr(idt_ptr);
	debug("IDT Add 0x%x\n",(unsigned int)&IDT_add);

	unsigned int add_BP_Handler;
	add_BP_Handler = (unsigned int)&bp_handler;
	debug("BPHandler Add 0x%x\n",add_BP_Handler);

	idt_ptr.desc[3].offset_1 = (unsigned int)&bp_handler & 0xFFFF;
	idt_ptr.desc[3].offset_2 = ((unsigned int)&bp_handler)>>16 & 0xFFFF;

	bp_trigger();


	debug("Bien revenu\n");


	// TODO call bp_trigger
   //bp_trigger();
}
