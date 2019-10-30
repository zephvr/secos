/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <intr.h>

extern info_t *info;

void bp_handler() {
  asm volatile("pusha");
  debug("/!\\ Catch #BP /!\\\n");
  // Display saved eip
  uint32_t saved_eip;
  asm volatile ("mov 4(%%ebp), %0" : "=r" (saved_eip) :: );
  debug("Saved EIP: 0x%x\n", saved_eip);

  asm volatile("popa");


  asm volatile("leave");
  asm volatile ("iret");
}

void bp_trigger() {
  debug("start trigger\n");
  asm volatile ("int3");
  debug("end trigger\n");
}

void tp()
{
  /*
  int a = 0, b=0;
  //for (int i=0; i<100; i++)
  a = a/b;
  debug("a=%d\n", a);
  */
  
  /* Question 3 */
  idt_reg_t idtr;
  get_idtr(idtr);
  debug("=== IDT @ %p ===\n", idtr.addr);

  offset_t isr = (offset_t)bp_handler;

 
  int_desc(&idtr.desc[3], gdt_krn_seg_sel(1), isr);
  /* modifier IDT */
  set_idtr(idtr);
  bp_trigger();
  debug("After trigger\n");

}
