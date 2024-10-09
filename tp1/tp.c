/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void print_gdt_content(gdt_reg_t gdtr_ptr) {
    seg_desc_t* gdt_ptr;
    gdt_ptr = (seg_desc_t*)(gdtr_ptr.addr);
    int i=0;
    while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit)) {
        uint32_t start = gdt_ptr->base_3<<24 | gdt_ptr->base_2<<16 | gdt_ptr->base_1;
        uint32_t end;
        if (gdt_ptr->g) {
            end = start + ( (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1) <<12) + 4095;
        } else {
            end = start + (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1);
        }
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("seg_t: 0x%x ", gdt_ptr->type);
        debug("desc_t: %d ", gdt_ptr->s);
        debug("priv: %d ", gdt_ptr->dpl);
        debug("present: %d ", gdt_ptr->p);
        debug("avl: %d ", gdt_ptr->avl);
        debug("longmode: %d ", gdt_ptr->l);
        debug("default: %d ", gdt_ptr->d);
        debug("gran: %d ", gdt_ptr->g);
        debug("\n");
        gdt_ptr++;
        i++;
    }
}



void set_gdt_entry(seg_desc_t *gdt, int num, uint32_t base, uint32_t limit, uint8_t type, uint8_t s, uint8_t dpl, uint8_t p, uint8_t avl, uint8_t l, uint8_t d, uint8_t g) {
    gdt[num].base_1 = base & 0xFFFF;
    gdt[num].base_2 = (base >> 16) & 0xFF;
    gdt[num].base_3 = (base >> 24) & 0xFF;

    gdt[num].limit_1 = (limit & 0xFFFF);
    gdt[num].limit_2 = (limit >> 16) & 0x0F;

    gdt[num].type = type;
    gdt[num].s = s;
    gdt[num].dpl = dpl;
    gdt[num].p = p;
    gdt[num].avl = avl;
    gdt[num].l = l;
    gdt[num].d = d;
    gdt[num].g = g;
}


void tp() {

    

	gdt_reg_t gdt_reg;
    get_gdtr(gdt_reg);

    print_gdt_content(gdt_reg);

    uint16_t cs,ds,ss,es,fs,gs;

    ss = get_ss();
    ds = get_ds();
    es = get_es();
    fs = get_fs();
    gs = get_gs();
    cs = get_cs();

    debug("SS:%d\n",ss);
    debug("DS:%d\n",ds);
    debug("ES:%d\n",es);
    debug("FS:%d\n",fs);
    debug("GS:%d\n",gs);
    debug("CS:%d\n",cs);

    debug("-----------------RECONFIG GDT----------------\n");

    __attribute__((aligned(8))) seg_desc_t gdt[3]; //NULL CODE DATA

    set_gdt_entry(gdt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    set_gdt_entry(gdt, 1, 0, 0xFFFFFFFF, 0xb, 1, 0, 1, 0, 0, 1, 1);
    set_gdt_entry(gdt, 2, 0, 0xFFFFFFFF, 0x3, 1, 0, 1, 0, 0, 1, 1);
    set_gdt_entry(gdt, 3, 0x600000, 0xFFFFFFFF, 0x3, 1, 0, 1, 0, 0, 1, 1);

    gdt_reg_t new_gdt_reg;

    new_gdt_reg.addr = (uint32_t)gdt;
    new_gdt_reg.limit = (sizeof(seg_desc_t) * 4) - 1;

    set_gdtr(new_gdt_reg);

    gdt_reg_t gdt_reg2;
    get_gdtr(gdt_reg2);

    set_cs(0x08);
    set_ds(0x10); 
    set_es(0x10);
    set_fs(0x10);
    set_gs(0x10);
    set_ss(0x10);

    ss = get_ss();
    ds = get_ds();
    es = get_es();
    fs = get_fs();
    gs = get_gs();
    cs = get_cs();

    debug("SS:%d\n",ss);
    debug("DS:%d\n",ds);
    debug("ES:%d\n",es);
    debug("FS:%d\n",fs);
    debug("GS:%d\n",gs);
    debug("CS:%d\n",cs);

    print_gdt_content(gdt_reg2);
    
}
