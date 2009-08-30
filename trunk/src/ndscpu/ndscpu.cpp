#include "ndscpu.h"

#include <cstring>

enum {
    UNDEFINED = 0
};

NDSCPU::NDSCPU(NDSMMU *ptr) : mem(ptr)
{
    u32 k;
    for(k = 0; k < 16; k++)
        m_r[k] = 0;
    for(k = 0; k < 23; k++)
        mode_saves[k] = 0;

    mode_spot[USER] = 0;
    mode_spot[SYSTEM] = 0;
    mode_spot[SUPER] = 8;
    mode_spot[ABORT] = mode_spot[SUPER] + 2;
    mode_spot[UNDEF] = mode_spot[ABORT] + 2;
    mode_spot[IRQ] = mode_spot[UNDEF] + 2;
    mode_spot[FIQ] = mode_spot[IRQ] + 2;

    cpsr.bit.mode = 0xDF;

    DCONLY = false;
}

void NDSCPU::modeswitch(u32 mode)
/*  *********************************************
    * This function is responsible for switching*
    * processor modes and saving and retrieving *
    * saved registers in the process for every  *
    * register necessary                        *
    *********************************************

    NOTE: Not quite sure if this way works better than
          hard coding everything by hand               */
{
    if( mode == USER && cpsr.bit.mode == SYSTEM ) return; /* User and System Modes share all registers, and you can't switch to SYSTEM from USER */

    if( mode != FIQ )
    {
        /* NOTE: This was changed from user and system modes saving 9 registers
                 to only saving them when switching to FIQ mode */

        if( cpsr.bit.mode == FIQ )
        {
            memcpy( &mode_saves[mode_spot[FIQ]], &m_r[8] , sizeof(u32) * 7 );

            memcpy( &m_r[8], &mode_saves, sizeof(u32) * 7 );
            if( mode != USER && mode != SYSTEM )
                memcpy( &m_r[13], &mode_saves[mode_spot[cpsr.bit.mode]], sizeof(u32) * 2 );
        }else{

            if( cpsr.bit.mode==SYSTEM || cpsr.bit.mode==USER )
            {
                memcpy( &mode_saves[5] , &m_r[13] , sizeof(u32) * 2);

            }else{
                /* Save the current mode in the 'mode_saves' array */
                memcpy( &mode_saves[mode_spot[cpsr.bit.mode]],   /* Spot in the saved register array, where to start saving to */
                        &m_r[13],                       /* Spot in the registers array to start saving from */
                        sizeof(u32) * 2                 /* Size of the memory being copied( 4 * #R's ) */ );

                /* Change r13, r14 to the mode apropriate values */
            }

            if( mode == SYSTEM || mode == USER )
                memcpy( &m_r[13], &mode_saves[5], sizeof(u32) * 2 );
            else
                memcpy(&m_r[13], &mode_saves[mode_spot[mode]], sizeof(u32) * 2 );
        }

    }else
        /* Fast Interrupt Mode uses its own registers( r8 - r14 ) */
    {
        if( cpsr.bit.mode == USER || cpsr.bit.mode == SYSTEM )
        {
            memcpy( &mode_saves, &m_r[8], sizeof(u32) * 7 );

            memcpy( &m_r[8], &mode_saves[mode_spot[FIQ]], sizeof(u32) * 7 );

        }else
        {
            /* We save register r8-r12 into the USER mode area */
            memcpy( &mode_saves,
                    &m_r[8],
                    sizeof(u32) * 5 );
            /* And the other 2 registers(r13, r14) are saved into its own space */
            memcpy( &mode_saves[mode_spot[mode]],
                    &m_r[13],
                    sizeof(u32) * 2 );

            memcpy( &m_r[8], &mode_saves[mode_spot[FIQ]], sizeof(u32) * 7 );
        }
    }
}

void NDSCPU::exec()
{
    if( DCONLY )
    {
        m_r[15] = m_newpc;
        if( cpsr.bit.t )
        {
            /* Thumb Mode */
            m_thumb_op.val = mem->read_hword(m_newpc);
            m_instruction = UNDEFINED;
            m_value = UNDEFINED;
            m_adress = UNDEFINED;
            thumb_decoder();
        }
        else
        {

            m_op.val = mem->read_word(m_newpc);
            m_instruction = UNDEFINED;
            m_value = UNDEFINED;
            m_adress = UNDEFINED;
            cpu_decoder();
        }
    }else{

        m_r[15] = m_newpc;
        if( cpsr.bit.t )
        {
            /* Thumb Mode */
            m_thumb_op.val = mem->read_hword(m_newpc);
            m_instruction = UNDEFINED;
            m_value = UNDEFINED;
            m_adress = UNDEFINED;
            thumb_decoder();
            thumb_executor();
        }
        else
        {

            m_op.val = mem->read_word(m_newpc);
            m_instruction = UNDEFINED;
            m_value = UNDEFINED;
            m_adress = UNDEFINED;
            cpu_decoder();
            cpu_executor();
        }
    }
}

int NDSCPU::m_write(u32& val, u32& adress, int sz)
{/* Parser for writing memory */

    /* Check if its writing to IO registers */
    if( adress > REGS_MIN && adress < REGS_MAX )
    {
        if( adress == 0x4000208 )
            interrupt = (val == 1) ? true : false;

    }

    return TRUE;
}

bool NDSCPU::iscond()
{
    switch( m_op.cond.val ){
        case EQ:
            return (this->cpsr.bit.z) ? true : false;
        case NE:
            return (cpsr.bit.z) ? false : true;
        case CS:
            return (cpsr.bit.c) ? true : false;
        case CC:
            return (cpsr.bit.c) ? false : true;
        case MI:
            return (cpsr.bit.n) ? true : false;
        case PL:
            return (cpsr.bit.n) ? false : true;
        case VC:
            return (cpsr.bit.v) ? true : false;
        case VS:
            return (cpsr.bit.v) ? false : true;
        case HI:
            return (cpsr.bit.c && !cpsr.bit.z) ? true : false;
        case LS:
            return (cpsr.bit.c && cpsr.bit.z) ? true : false;
        case GE:
            return (cpsr.bit.n == cpsr.bit.v) ? true : false;
        case LT:
            return (cpsr.bit.n != cpsr.bit.v) ? true : false;
        case GT:
            return (!cpsr.bit.z && cpsr.bit.n == cpsr.bit.v) ? true : false;
        case LE:
            return (cpsr.bit.z && cpsr.bit.n != cpsr.bit.v) ? true : false;
        case AL:
            return true;
    }

    return false;
}
