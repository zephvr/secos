/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>

extern info_t *info;

#define c0_idx  1
#define d0_idx  2

#define c0_sel  gdt_krn_seg_sel(c0_idx)
#define d0_sel  gdt_krn_seg_sel(d0_idx)

void print_SD(seg_desc_t *sd) {
  debug("------ GDT ENTRY ------\n");
  debug("|@Address : %p\n", sd);
  if (sd->p == 0) {
    debug("| /!\\ ENTRY EMPTY /!\\\n");
  }
  debug("| limit_1 : %p\n", sd->limit_1);
  debug("|  base_1 : %p\n", sd->base_1);
  debug("|  base_1 : %p\n", sd->base_2);
  debug("|    type : %p\n", sd->type);
  debug("|       s : b%x\n", sd->s);
  debug("|     dpl : %p\n", sd->dpl);
  debug("|       p : b%x\n", sd->p);
  debug("| limit_2 : %p\n", sd->limit_2);
  debug("|     avl : b%x\n", sd->avl);
  debug("|       l : b%x\n", sd->l);
  debug("|       d : b%x\n", sd->d);
  debug("|       g : b%x\n", sd->g);
  debug("|  base_3 : %p\n", sd->base_3);
  debug("---- END GDT ENTRY ----\n");

}

void tp()
{
  /* Question 1 */
  gdt_reg_t gdt;
  get_gdtr(gdt);
  debug("@Address of GDT : %p %d\n", &gdt, sizeof(seg_desc_t));

  seg_desc_t *desc = gdt.desc;
  for (int i=0; i < (gdt.limit + 1); i = i + sizeof(seg_desc_t)) {
    print_SD(desc);
    desc++;
  }

  /* Question 2 */
  //memset(&gdt, 0, sizeof(gdt));
  //seg_desc_t descriptors[3];
  //memset(descriptors, 0, sizeof(seg_desc_t) * 3);
  seg_desc_t GDT[6];
  GDT[0].raw = 0ULL;

  // First descriptors
  // ring 0, code RX flat
  
  GDT[1].raw = 0;
  GDT[1].limit_1 = 0xffff;
  GDT[1].limit_2 = 0xf;
  GDT[1].type = SEG_DESC_CODE_XR; // code rx
  GDT[1].dpl = 0;
  GDT[1].d = 1; // default opération size (0:16, 1:32 nit segment)
  GDT[1].g = 1; // granualité (0:1b, 1:4kb)
  GDT[1].s = 1; // not system segment
  GDT[1].p = 1; // présence flag

  // Third descriptors
  // ring 0, code RX flat
  GDT[2].raw = 0;
  GDT[2].limit_1 = 0xffff;
  GDT[2].limit_2 = 0xf;
  GDT[2].type = SEG_DESC_DATA_RW; // code rw
  GDT[2].dpl = 0;
  GDT[2].d = 1; // default opération size (0:16, 1:32 nit segment)
  GDT[2].g = 1; // granualité (0:1b, 1:4kb)
  GDT[2].s = 1; // not system segment
  GDT[2].p = 1; // présence flag
  
  gdt.desc = GDT;
  gdt.limit = sizeof(GDT)-1;
  //memcpy(gdt.desc, descriptors, sizeof(seg_desc_t)*3);
  set_gdtr(gdt); // set GDT table

  uint16_t ss = get_ss(), ds = get_ds(), es = get_es(), fs = get_fs(), gs = get_gs();
  debug("ss = %p, ds = %p, es = %p, fs = %p, gs = %p\n", ss, ds, es, fs, gs);

  // Set segmentation register
  set_cs(c0_sel);

  set_ss(d0_sel);
  set_ds(d0_sel);
  set_es(d0_sel);
  set_fs(d0_sel);
  set_gs(d0_sel);


  // Print new GDT
  debug("=============================\n");
  get_gdtr(gdt);
  debug("@ADDRESS of GDT: 0x%x\n", &gdt);

  desc = gdt.desc;
  for (int i=0; i < (gdt.limit + 1); i = i + sizeof(seg_desc_t)) {
    print_SD(desc);
    desc++;
  }

  ss = get_ss(), ds = get_ds(), es = get_es(), fs = get_fs(), gs = get_gs();
  debug("ss = %p, ds = %p, es = %p, fs = %p, gs = %p\n", ss, ds, es, fs, gs);

  /* Question 3 */
  
  char src[64];
  char *dst = 0;
  memset(src, 0xff, 64);

  
  get_gdtr(gdt); desc = gdt.desc;
  desc++; desc++; desc++;
  // index 3
  desc->raw = 0;
  desc->limit_1 = 31; /// 32 - 1
  desc->limit_2 = 0x0;
  desc->base_1 = 0x0000;
  desc->base_2 = 0x60;
  desc->base_3 = 0x00;
  desc->type = SEG_DESC_DATA_RW; // Code RX
  desc->d = 1;
  desc->s = 1; // not system segment
  desc->p = 1; // présence flag
  desc->dpl = 0; // ring 0
  desc->g = 0; // granualité sur 1B
  desc->avl = 0;
  desc->l = 0;
  gdt.limit = sizeof(seg_desc_t)*4-1;
  set_gdtr(gdt); // set GDT table

  set_es(gdt_krn_seg_sel(3));
  _memcpy8(dst, src, 32);

  debug("%p, %p\n", dst, *dst);

  ss = get_ss(), ds = get_ds(), es = get_es(), fs = get_fs(), gs = get_gs();
  debug("ss = %p, ds = %p, es = %p, fs = %p, gs = %p\n", ss, ds, es, fs, gs);
  
}
