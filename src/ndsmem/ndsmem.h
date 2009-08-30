#ifndef NDSMEM_H
#define NDSMEM_H

#include "../types.h"
#include "cp15defines.h"


struct tcmregion_s{
/* This structure is also used for protection regions */
    u32 base_adress;
    u32 size;
    u32 enabled; /* 0/1 */
    u32 limit;
    u32 buffered; /* 0/1 */
};

/* The 'Memory Managament Unit' takes on a quite broad meaning in this representation */
class NDSMMU
{
    public:

        NDSMMU();
        virtual ~NDSMMU(){};

        int write_word(u32& val, u32& adress){return write_mem(val, adress, sizeof(u32));}
        u32 read_word(u32& adress);
        u16 read_hword(u32& adress);

        /* NOTE: Should there be even a need for all these */
        int write_mem(u32& val, u32& adress, int sz);

        int write_halfword(u32 val, u32& adress){ return write_mem(val, adress, sizeof(u16));}
        int write_byte(u32 val, u32& adress){ return write_mem(val, adress, sizeof(u8));}

        void mcr(u32 op1,u32 rd,u32 crn,u32 crm,u32 op2);
        u32 mrc(u32 op1,u32 rd,u32 crn,u32 crm,u32 op2);
        /* For convenience only */
        void set_cc_val(int r, u32 val){
            switch(r)
            {
                case 1:
                    cc01.val = val;
                    break;
            }
        };

        u8 *memory;
        u8 *ioregs;

        u8 m_itcm[0x8000];
        u8 m_dtcm[0x4000];
        u8 m_main[0x1000000];/* NOT actual size */
        u8 m_wram[0x1000000];
        u8 m_regs[0x1000000];
        u8 m_stack[0x1000000];
        u8 m_palt[0x800];
        u8 m_video[0x1000000];
        u8 m_sprt[0x800];
        u8 m_bios[0x8000];
        u32 exception_loc; /* Location of Exception vectors, register c1( 0x0000 0000 or 0xFFFF 0000 ) */

    protected:



        struct tcmregion_s itcm;
        struct tcmregion_s dtcm;

        struct tcmregion_s pregion[8];

        union CPREG000_U cc00;
        union CPREG001_U cc01;
        union CPREG002_U cc02;
        union CPREG100_U cc1;
        u32              cc3;
        union CPREG500_U cc5i, cc5d;
        union CPREG600_U cc6;
        union CPREG900_U cc9i, cc9d;

};

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

#endif
