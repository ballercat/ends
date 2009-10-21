#ifndef ENDSMEMORYH
#define ENDSMEMORYH

#include "../types.h"

typedef struct ndsMEM
{
	u8	*ITCM;
	u8	*DTCM;
	u8	*MAIN;
	u8	*WRAM;
	u8	*REGS;
	u8	*STACK;
	u8	*PALT;
	u8	*VIDEO;
	u8	*SPRT;
	u8	*BIOS;
	u32	expt_loc;
	
	u8	*_main;	/* main memory */
}ndsMEM;

extern struct ndsMEM *init_memory_arm9(void);
extern void destroy_memory_arm9(struct ndsMEM *mem);
extern struct ndsMEM *init_memory_arm7(void);

extern u32  read_word(u32 adr, ndsMEM *mem);
extern u16  read_halfword(u32 adr, ndsMEM *mem);
extern u8   read_byte(u32 adr, ndsMEM *mem);
extern void write_word(u32 adr, u32 val, ndsMEM *mem);
extern void write_halfword(u32 adr, u16 val, ndsMEM *mem);
extern void write_byte(u32 adr, u8 val, ndsMEM *mem);

#define ITCM_SIZE   0x8000
#define DTCM_SIZE   0x4000
#define MAIN_SIZE   0x400000
#define WRAM_SIZE   0x8000
#define REGS_SIZE   0x1000000
#define PALETTE_SIZE    0x800
#define VIDEO_SIZE  0x1000000
#define SPRITE_SIZE 0x800
#define BIOS_SIZE   0x8000

#define ITCM_MAX    0x00008000
#define ITCM_MIN    0
#define DTCM_MAX    0
#define DTCM_MIN    0
#define TCM_MAX     0x02000000
#define TCM_MIN     0
#define MAIN_MAX    0x03000000
#define MAIN_MIN    0x02000000
#define WRAM_MAX    0x04000000
#define WRAM_MIN    0x03000000
#define REGS_MAX    0x05000000
#define REGS_MIN    0x04000000
#define PALETTE_MAX 0x06000000
#define PALETTE_MIN 0x05000000
#define VIDEO_MAX   0x07000000
#define VIDEO_MIN   0x06000000
#define SPRITE_MAX  0x08000000
#define SPRITE_MIN  0x07000000
#define BIOS_MAX    0x100000000ULL
#define BIOS_MIN    0xFFFF0000

#define STACK_MIN   0x08000000
#define STACK_MAX   0x09000000

#endif //ENDSMEMORYH

