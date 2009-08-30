#include "ndsmem.h"
#include <cstring>
#include <cstdlib>

u32 regionsize[] = {0, 0, 0, 0x1000, 0x2000, 0x4000, 0x8000,
                0x10000, 0x20000, 0x40000, 0x80000,
                0x100000, 0x200000, 0x400000, 0x800000,
                0x1000000, 0x2000000, 0x4000000, 0x8000000,
                0x10000000, 0x20000000, 0x40000000, 0x80000000, 1 };

NDSMMU::NDSMMU() : memory(m_main),ioregs(m_regs)
{
    this->itcm.enabled = 0;
    this->itcm.base_adress = 0x0000;
    this->itcm.size = sizeof(m_itcm);
    this->itcm.limit = this->itcm.size;

    this->dtcm.enabled = 0;
    this->dtcm.base_adress = 0x0000;
    this->dtcm.size = sizeof(m_dtcm);
    this->dtcm.limit = this->dtcm.size + this->dtcm.base_adress;

    memset(&m_main, 0, sizeof(m_main));
    memset(&m_stack, 0, sizeof(m_main));

}
/* A litle less logic in reading since its done more */
u32 NDSMMU::read_word(u32& adress)
{
    u32 ret = 0;
    if( adress < MAIN_MAX && adress >= MAIN_MIN )
        memcpy(&ret, &m_main[adress - MAIN_MIN], sizeof(u32));
    else if( adress < ITCM_MAX )
    {
        if( !itcm.enabled ) ret = 0x0000;
        memcpy(&ret, &m_itcm[adress], sizeof(u32));
    }
    else if( adress < dtcm.limit && adress > dtcm.base_adress )
    {
        if( !dtcm.enabled ) ret = 0x0000;
        memcpy(&ret, &m_dtcm[adress - dtcm.base_adress], sizeof(u32));
    }
    else if( adress < WRAM_MAX && adress >= WRAM_MIN )
        memcpy(&ret, &m_wram[adress - WRAM_MIN], sizeof(u32));
    else if( adress < REGS_MAX && adress >= WRAM_MIN )
    {    /* I/O Registers */
        memcpy(&ret, &m_regs[adress - REGS_MIN], sizeof(u32));

    }
    else if( adress >= BIOS_MIN )
        memcpy(&ret, &m_bios[adress - BIOS_MIN], sizeof(u32));

    else if( adress >= STACK_MIN && adress < STACK_MAX )
    {
        memcpy(&ret, &m_stack[adress - STACK_MIN], sizeof(u32));
        memset(&m_stack[adress - STACK_MIN], 0, sizeof(u32));
    }
    return ret;
}

u16 NDSMMU::read_hword(u32& adress)
{
    u16 ret = 0;
    if( adress < MAIN_MAX && adress >= MAIN_MIN )
        memcpy(&ret, &m_main[adress - MAIN_MIN], sizeof(u16));
    else if( adress < ITCM_MAX )
    {
        if( !itcm.enabled ) ret = 0x0000;
        memcpy(&ret, &m_itcm[adress], sizeof(u32));
    }
    else if( adress < dtcm.limit && adress > dtcm.base_adress )
    {
        if( !dtcm.enabled ) ret = 0x0000;
        memcpy(&ret, &m_dtcm[adress - dtcm.base_adress], sizeof(u16));
    }
    else if( adress < WRAM_MAX && adress >= WRAM_MIN )
        memcpy(&ret, &m_wram[adress - WRAM_MIN], sizeof(u16));
    else if( adress < REGS_MAX && adress >= WRAM_MIN )
    {    /* I/O Registers */
        memcpy(&ret, &m_regs[adress - REGS_MIN], sizeof(u16));

    }
    else if( adress >= BIOS_MIN )
        memcpy(&ret, &m_bios[adress - BIOS_MIN], sizeof(u16));

    return ret;
}

int NDSMMU::write_mem(u32& val, u32& adress, int sz)
{
    /* WIP TCM */
    if( adress < itcm.size && itcm.enabled )
    {/* Instruction Tightly Coupled Memory */
        memcpy(&m_itcm[adress], &val, sz);
        return TRUE;
    }
    if( adress > dtcm.base_adress && adress < dtcm.limit )
    {/* Data Tightly Coupled Memory */
        memcpy(&m_dtcm[adress - dtcm.base_adress], &val, sz);
        return TRUE;
    }
    /* RAM */
    if( adress < MAIN_MAX && adress >= MAIN_MIN ){
        int k, mod = ((adress - MAIN_MIN)/MAIN_SIZE);
        for ( k = 0 ; k < mod; k++ )
            memcpy(&m_main[(k * MAIN_SIZE) + adress - MAIN_MIN ], &val, sz);
        return TRUE;
    }
    if( adress < WRAM_MAX && adress >= WRAM_MIN ){
        int k, mod = ((adress - WRAM_MIN)/WRAM_SIZE);
        for( k = 0 ; k < mod; k++ )
            memcpy(&m_wram[(k * WRAM_SIZE) + adress - WRAM_MIN], &val, sz);
        return TRUE;
    }
    if( adress < REGS_MAX && adress >= REGS_MIN ){
        memcpy(&m_regs[adress - REGS_MIN], &val, sizeof(u32));

        if( (adress - REGS_MIN) == 0x4000000 ) exit(EXIT_SUCCESS);
        return TRUE;
    }

    if( adress < STACK_MAX && adress >= STACK_MIN ){
        memcpy(&m_stack[adress - STACK_MIN], &val, sizeof(u32));
        return TRUE;
    }
    /* VRAM */

    return FALSE;
}

/* MCR( Move Register to Coprocessor ) */
void NDSMMU::mcr(u32 op1, u32 rd, u32 crn, u32 crm, u32 op2)
{
    switch( crn )
    {
        case 1:
            /* Control Register 1 c1 */
            memcpy(&this->cc1.bit, &rd, sizeof(u32));
            exception_loc = cc1.bit.vbit * 0xFFFF0000;
            itcm.enabled = cc1.bit.itcm_enable;
            dtcm.enabled = cc1.bit.dtcm_enable;
            return;
        case 3:
            memcpy(&cc3, &rd, sizeof(u32));
            pregion[0].buffered = ( cc3 & 1 ) ;
            pregion[1].buffered = ( cc3 & 2 ) ? 1 : 0;
            pregion[2].buffered = ( cc3 & 4 ) ? 1 : 0;
            pregion[3].buffered = ( cc3 & 8 ) ? 1 : 0;
            pregion[4].buffered = ( cc3 & 0x10 ) ? 1 : 0;
            pregion[5].buffered = ( cc3 & 0x20 ) ? 1 : 0;
            pregion[6].buffered = ( cc3 & 0x40 ) ? 1 : 0;
            pregion[7].buffered = ( cc3 & 0x80 ) ? 1 : 0;
            return;
        case 5:
            if( op2 == 3 )
                memcpy(&cc5i, &rd, sizeof(u32));
            else if( op2 == 2 )
                memcpy(&cc5d, &rd, sizeof(u32));
            return;
        case 6:
            memcpy(&cc6.val, &rd, sizeof(u32));

            pregion[crm].enabled = cc6.bit.enabled;
            pregion[crm].base_adress = (cc6.bit.base << 12);

            if( cc6.bit.size == 1 ) /* 4 GB region */
            {
                pregion[crm].base_adress = 0;
                pregion[crm].size = 0xFFFFFFFF + 1;
                pregion[crm].limit = pregion[crm].size;
                return;
            }

            pregion[crm].size = regionsize[cc6.bit.size];
            pregion[crm].limit = pregion[crm].base_adress + pregion[crm].size;

            return;
        /* Cache Operations Register c7  <-- is not needed and does not exist */
        case 9:
        /* Cache Lockdown register does not exist */
            if( crm == 1 )
            {
                if( op2 ){
                    memcpy(&this->cc9i.val, &rd, sizeof(u32));
                    itcm.enabled = cc9i.bit.enabled;
                }else{
                    memcpy(&this->cc9d.val, &rd, sizeof(u32));
                    dtcm.enabled = cc9d.bit.enabled;
                    dtcm.base_adress = (cc9d.bit.base_adress << 12);
                    dtcm.limit = dtcm.base_adress + dtcm.size;
                }
                /* Crash the system if data tcm and instruction tcm base adresses match  && cc9d.bit.base_adress == cc9i.bit.base_adress */
                if( dtcm.enabled && !dtcm.base_adress ) exit(EXIT_FAILURE);
                return;
            }
            return;
    }
}

/* MRC(Move Register from Coprossesor) */
u32 NDSMMU::mrc(u32 op1,u32 rd,u32 crn,u32 crm,u32 op2)
{
    switch( crn )
    {
        case 0: break;
        case 1:
            if( !crm && !op2 ) return cc1.val;
    };

    return FALSE;
}

