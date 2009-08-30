#include "ndscpu.h"
#include "instructions.h"



void NDSCPU::thumb_executor()
{
    u32 math_result = 0;
    switch( m_instruction & mINSTRUCTION )
    {
        case TBR:
            if( m_instruction & OFFSETt )
                if( !iscond() ) break;
            m_r[15] = m_adress;
            break;
        case TBX:
            m_op.R.m = ( m_thumb_op.F.h1  << 4 ) | m_thumb_op.R.nm ;
            cpsr.bit.t = m_r[m_op.R.m] & 1;
            m_adress = (m_r[m_op.R.m] & 0xFFFFFFFE);
            m_r[15] = m_adress - 2;
            break;
        case TBL:
            m_r[14] = m_r[15] + 4;

            m_r[15] = m_adress;

            break;
        case TPUSH:
            {
                u32 rc = 0;
                int k;
                for( k = 0; k < 8; k ++ )
                    if( imRlist( m_thumb_op.val , k) ) rc++;

                u32 start_adress = m_r[13] - 4 * ( m_thumb_op.F.lx + rc );

                rc = 0;
                for( k = 0; k < 8; k ++ )
                {
                    if( imRlist( m_thumb_op.val , k) )
                    {
                        mem->write_halfword(start_adress + (rc * 4), m_r[k]);
                        rc++;
                    }
                }
                if( m_thumb_op.F.lx )
                    mem->write_halfword(start_adress + (rc * 4), m_r[13]);

                m_r[13] = start_adress;
                break;
            }
            break;
        case TPOP:
            {
                u32 rc = 0;
                int k;
                for( k = 0; k < 8; k ++ )
                    if( imRlist( m_thumb_op.val, k) ) rc++;

                u32 end_adress = m_r[13] - 4 * (m_thumb_op.F.lx + rc);

                rc = 0;
                for( k = 0; k < 8; k ++ )
                {
                    if( imRlist( m_thumb_op.val , k ) )
                    {
                        m_adress = end_adress - (rc * 4);
                        m_r[k] = mem->read_word(m_adress);
                        rc++;
                    }
                }
                m_r[13] = end_adress;
                if( m_thumb_op.F.lx )
                {
                    m_adress = end_adress - (rc * 4);
                    m_r[15] = mem->read_word(m_adress );
                    if( !(m_r[15] & 1) )
                    {
                        m_newpc = m_r[15];
                        cpsr.bit.t = 0;
                        return;
                    }
                }
                break;
            }
        case TMOV:
            m_r[m_op.R.d] = m_value;
            if( m_adress )
            {
                cpsr.bit.n = ( m_r[m_op.R.d] & SIGNBIT ) ? 1 : 0;
                cpsr.bit.z = ( !m_r[m_op.R.d] ) ? 1 : 0;
                if( m_op.F.s )
                {
                    cpsr.bit.c = 0;
                    cpsr.bit.v = 0;
                    m_op.F.s = 0;
                }
            }
            break;
        case TLDR:
            if( !(m_adress & 0x1) )
                m_r[m_op.R.d] = mem->read_word(m_adress);
            break;
        case TCMP:
            math_result = m_r[m_op.R.d] - m_value;
            cpsr.bit.n = ( math_result & SIGNBIT ) ? 1 : 0;
            cpsr.bit.z = ( !math_result ) ? 1 : 0;
            cpsr.bit.c = !(SET_CARRY( m_r[m_op.R.d], m_value, math_result ));
            cpsr.bit.v = SET_OVERFLOW( m_r[m_op.R.d], m_value, math_result );
            break;
    }

    m_newpc = m_r[15] + 2;
}
