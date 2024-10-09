/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   multiboot_memory_map_t* entry = (multiboot_memory_map_t*)info->mbi->mmap_addr;
   while((uint32_t)entry < (info->mbi->mmap_addr + info->mbi->mmap_length)) {
      // TODO print "[start - end] type" for each entry

      uint64_t start = entry->addr;
      uint64_t end = entry->addr + entry->len - 1;
      const char* type;


      switch (entry->type) {
         case MULTIBOOT_MEMORY_AVAILABLE:
            type = "MULTIBOOT_MEMORY_AVAILABLE";
            break;
         case MULTIBOOT_MEMORY_RESERVED:
            type = "MULTIBOOT_MEMORY_RESERVED";
            break;
         case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            type = "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE";
            break;
         case MULTIBOOT_MEMORY_NVS:
            type = "MULTIBOOT_MEMORY_NVS";
            break;
         default:
            type = "UNKNOWN";
            break;
      }



      debug("[0x%llx - 0x%llx] %s\n", start, end, type);
      entry++;
   }

   int *ptr_in_available_mem;
   ptr_in_available_mem = (int*)0x0;
   debug("Available mem (0x0): before: 0x%x ", *ptr_in_available_mem); // read
   *ptr_in_available_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_available_mem);                // check

   int *ptr_in_reserved_mem;
   ptr_in_reserved_mem = (int*)0xf0000;
   debug("Reserved mem (at: 0xf0000):  before: 0x%x ", *ptr_in_reserved_mem); // read
   *ptr_in_reserved_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_reserved_mem);                // check

   int *ptr_in_oversized_mem;
   ptr_in_oversized_mem = (int*)0xfffffff;
   debug("Oversized mem (at: 0xfffffffff):  before: 0x%x ", *ptr_in_oversized_mem); // read
   *ptr_in_oversized_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_oversized_mem);                // check


}
