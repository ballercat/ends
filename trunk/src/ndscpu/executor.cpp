#include "ndscpu.h"
#include "instructions.h"
#include <math.h>

/* TODO: Add COMMENTS! */

/* Function: class::cpu_executor()
    Parameters: none
    note: Called in the main cpu execution loop, after decoding is done */
void NDSCPU::cpu_executor()
{
    /* Instructions */
    if( iscond() )
    {
        u32 math_result = 0;

        switch(m_instruction & mINSTRUCTION)
        {
            case MOV:
                m_mov();
                break;
            case LDR:
                m_ldr();
                break;
            case STR:
                break;
            case CMP:
                break;
            case BR:
            case BL:
            case BLX:
            case BX:
                m_branch();
                break;
            case MRC:
                m_r[m_op.R.d] = mem->mrc(m_op.CP.op1, m_r[m_op.R.d], m_op.CP.crn, m_op.CP.crm, m_op.CP.op2 );
                break;
            case MCR:
                mem->mcr(m_op.CP.op1, m_r[m_op.R.d], m_op.CP.crn, m_op.CP.crm, m_op.CP.op2 );
                break;
            case MSR:
                m_msr();
                break;
            case MRS:
                if( m_op.F.b )
                    m_r[m_op.R.d] = spsr.val;
                else
                    m_r[m_op.R.d] = cpsr.val;

                break;
            case STRH:
                m_strh();
                break;
            case STM_1:
            {
                int k , c = 0, i = ( m_op.F.u ) ? 4 : -4;

                m_adress = m_r[m_op.R.n];
                if( !m_op.F.p ) m_adress += i;
                for( k = 0; k < 16; k++ )
                {
                    if( imRlist(m_op.val, k) )
                    {
                        mem->write_word(m_r[k], m_adress);
                        m_adress += i;
                        c++;
                    }
                }
                if( m_op.F.w )
                    m_r[ m_op.R.n ] = m_r[ m_op.R.n ] + (c * i);
                break;
            }
            case LDM_1:
            {
                int k, c = 0, i = ( m_op.F.u ) ? 4 : -4;
                m_adress = m_r[m_op.R.n];
                for( k = 0; k < 15; k++)
                {
                    if( imRlist(m_op.val, k) )
                    {
                        m_r[k] = mem->read_word(m_adress);
                        m_adress += i;
                        c++;
                    }
                }
                if( m_op.F.w && iscond() )
                    m_r[m_op.R.n] = m_r[m_op.R.n] + (c * i);

                break;
            }
            case SWI:
                m_swi();
                break;
            case BIC:
                m_r[m_op.R.d] = m_r[m_op.R.n] & (~m_value);
                if( m_op.F.s )
                {
                    cpsr.bit.n = ( m_r[m_op.R.d] & SIGNBIT ) ? 1 : 0;
                    cpsr.bit.z = ( m_r[m_op.R.d] ) ? 0 : 1;
                }
                break;
            case ADD:
                math_result = m_r[m_op.R.n] + m_value;
                break;
            case SUB:
                math_result = m_r[m_op.R.n] - m_value;
                break;
            case ORR:
                math_result = m_r[m_op.R.n] | m_value;
                break;
        }

        switch(m_instruction & mINSTRUCTION)
        {
            case ADD:
            case SUB:
                if( m_op.F.s )
                {
                    cpsr.bit.n = ( math_result & SIGNBIT ) ? 1 : 0;
                    cpsr.bit.z = ( !math_result ) ? 1 : 0;
                    cpsr.bit.c = SET_CARRY( m_r[m_op.R.n], m_value, math_result);
                    if( (m_instruction & mINSTRUCTION) == SUB ) cpsr.bit.c = !cpsr.bit.c;
                    cpsr.bit.v = SET_OVERFLOW( m_r[m_op.R.n] , m_value, math_result) ;
                }
                m_r[m_op.R.d] = math_result;
                break;
            case ORR:
                if( m_op.F.s )
                {
                    cpsr.bit.n = ( math_result & SIGNBIT ) ? 1 : 0;
                    cpsr.bit.z = ( !math_result ) ? 1 : 0;
                    cpsr.bit.c = SET_CARRY( m_r[m_op.R.n], m_value, math_result);
                }
                m_r[m_op.R.d] = math_result;
                break;
        }
    }
    m_newpc = m_r[15] + 4;
}
/* ------------------------------------------------------------------------------------------------------------ */
void NDSCPU::m_mov()
/* ------------------------------------------------------------------------------------------------------------ */
{
    m_r[m_op.R.d] = m_value;
    if( m_op.R.d == 15 )
        cpsr = spsr;
    if( m_op.F.s ){
        cpsr.bit.n = ( m_value & SIGNBIT ) ? 1 : 0;
        cpsr.bit.z = ( !m_value ) ? 1 : 0;
    }
}

/* Instruction : 'Load Register' */
/* ------------------------------------------------------------------------------------------------------------ */
void NDSCPU::m_ldr(){
/* ------------------------------------------------------------------------------------------------------------ */
/* Source 'Arm Architecture Reference Manual' page A4 - 37 */
    if( m_adress & 0x02 ) m_adress = m_adress >> (8 * (m_adress & 0x02));

    m_r[m_op.R.d] = mem->read_word(m_adress);

   /* if( m_op.R.d == PC ){
            if( m_r[PC] & 1 )
                // Switch/stay to/in Thumb
            else
                // Switch/stay to/in ARM
    } */
}
/* ------------------------------------------------------------------------------------------------------------ */
void NDSCPU::m_strh()
/* ------------------------------------------------------------------------------------------------------------ */
{
    if( m_instruction & Rt )
        mem->write_halfword( (m_r[m_op.R.d] & 0xFFFF), m_adress );
    else{/* FIXME: Below is a fast hack, add proper decoding */
        m_adress = (( m_op.R.s << 4 )| m_op.R.m ) + m_r[m_op.R.n];
        mem->write_halfword( (m_r[m_op.R.d] & 0xFFFF), m_adress );
    }

}

/* ------------------------------------------------------------------------------------------------------------ */
void NDSCPU::m_msr()
/* ------------------------------------------------------------------------------------------------------------ */
{
    u32 copy = ( m_op.F.b ) ? spsr.val : cpsr.val;
    if( m_op.F.b )
        if( cpsr.bit.mode == USER || cpsr.bit.mode == SYSTEM ) return;
	/* Any write to first 24 bits in User mode should be ignored */
	if( m_op.F.b && cpsr.bit.mode == USER )
		m_value = ( m_value & 0xFF000000 );
    if( (m_op.val & 0xF0000) != 0xF0000 )
    {
        if( m_op.val & 0x40000 ) copy = (copy & 0xFFFFF1F) | (m_value & 0xE0);
        if( m_op.val & 0x10000 ) copy = (copy & 0xFFFFFE0) | (m_value & 0x1F);
        if( m_op.val & 0x80000 ) copy = (copy & 0x7FFFFFF) | (m_value & 0xF8000000);
        m_value = copy;
    }
    if( m_op.val & 0x10000 ) modeswitch( m_value & 0x1F );

    if( m_op.F.b )
        spsr.val = m_value;
    else
        cpsr.val = m_value;
}
/* ------------------------------------------------------------------------------------------------------------ */
void NDSCPU::m_branch()
/* ------------------------------------------------------------------------------------------------------------ */
{
    u32 branchto = m_op.b24.val;
    switch( m_instruction & mINSTRUCTION )
    {
        case BLX:
        case BX:
            if( m_op.F.p )
                m_r[14] = m_r[15] + 4;

            branchto = m_r[m_op.R.m];
            cpsr.bit.t = branchto & 1;
            m_r[15] = branchto & 0xFFFFFFFE;
            return;
        case BR:
        case BL:
            if( branchto & 0x800000 ) branchto += 0xFF000000;
            branchto = (branchto << 2) + m_r[15] + 4;
            if( m_op.F.p ) /* bit 'L' */
            m_r[14] = m_r[15] + 4;
            m_r[15] = branchto & 0xFFFFFFFD;
            break;
    }
}

/* ------------------------------------------------------------------------------------------------------------ */
void NDSCPU::m_swi()
{




}
