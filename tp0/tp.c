/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   /* Question 2 */
   memory_map_t *start, *end;
   start = (memory_map_t*) info->mbi->mmap_addr;
   end = (memory_map_t*) (info->mbi->mmap_addr + info->mbi->mmap_length);
   for (; start < end; start++) {
     debug("Memory map: (addr=0x%llx, size=0x%x, len=0x%llx, type=%p)\n", start->addr,  start->size, start->len, start->type);
   }

   /* Question 3 */
   int *type_1 = (int *) 0x200000;
   int *type_2 = (int *) 0x900000;
   *type_1 = 666;
   *type_2 = 777;
   debug("Write type 1 : %d, type_2 : %d\n", *type_1, *type_2);

   /* Question 4 */
   int *out_of_memory = (int *) 0xf000000;
   *out_of_memory = 888;
   debug("Out of memory : %d\n", *out_of_memory);
}
