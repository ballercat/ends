#include "ndscpu.h"
#include "instructions.h"
#include "opcode.h"
#include <cstdlib>

static u32 rotater(u32 value, u8 rot)
{
    bool carry = false;
    for(int k = 0; k < rot; k++)
    {
        if( value & 0x01 ) carry = true;
        value >>= 1;
        value = (carry) ? (value | SIGNBIT ) : value;
        carry = false;
    }

    return value;
}

void NDSCPU::cpu_decoder()
{
    if( !m_op.val ) m_instruction = AND;

    /* While loop here works like a makeshift 'goto' */
    while( !m_instruction )
    {
        /* Regular arm9 Instructions */
        if((m_op.val & 0xE1000F0) == 0x00000B0){ m_instruction = STRH; break; }
        switch(m_op.val & 0xDE00000)
        {
            case 0x0000000: m_instruction = AND; break;
            case 0x0200000: m_instruction = EOR; break;
            case 0x0400000: m_instruction = SUB; break;
            case 0x0600000: m_instruction = RSB; break;
            case 0x0800000: m_instruction = ADD; break;
            case 0x0A00000: m_instruction = ADC; break;
            case 0x0C00000: m_instruction = SBC; break;
            case 0x0E00000: m_instruction = RSC; break;
            case 0x1400000: m_instruction = CMP; break;
            case 0x1800000: m_instruction = ORR; break;
            case 0x1A00000: m_instruction = MOV; break;
            case 0x1C00000: m_instruction = BIC; break;
        }
        if( m_instruction ) break;
        if((m_op.val & 0xC500000) == 0x4100000){ m_instruction = LDR; break; }
        if((m_op.val & 0xC500000) == 0x4500000){ m_instruction = LDRB; break; }
        if((m_op.val & 0xD700000) == 0x6700000){ m_instruction = LDRBT; break; }

        if((m_op.val & 0xF000000) == 0xA000000){ m_instruction = BR;  break; }
        if((m_op.val & 0xF000000) == 0xB000000){ m_instruction = BL;  break; }
        if((m_op.val & 0xFFF000F0)==0xE1A00070){ m_instruction = BKPT; break; }
        if((m_op.val & 0xFE000000)==0xFA000000){ m_instruction = BLX; break; }
        if((m_op.val & 0xFFFFFF0) == 0x12FFF30){ m_instruction = BLX; break; }
        if((m_op.val & 0xFFFFFF0) == 0x12FFF10){ m_instruction = BX;  break; }
        if((m_op.val & 0xF000010) == 0xE000000){ m_instruction = CDP; break; }
        if((m_op.val & 0xFFF0FF0) == 0x16F0F10){ m_instruction = CLZ; break; }
        if((m_op.val & 0xDF0F000) == 0x3700000){ m_instruction = CMN; break; }
        if((m_op.val & 0xE100000) == 0xC100000){ m_instruction = LDC; break; }
        if((m_op.val & 0xE500000) == 0x8100000){ m_instruction = LDM_1; break; }
        if((m_op.val & 0xE708000) == 0x8500000){ m_instruction = LDM_2; break; }
        if((m_op.val & 0xE508000) == 0x8508000){ m_instruction = LDM_3; break; }

        if((m_op.val & 0xE1000F0) == 0x01000B0){ m_instruction = LDRH; break; }
        if((m_op.val & 0xE1000F0) == 0x01000D0){ m_instruction = LDRSB; break; }
        if((m_op.val & 0xE1000F0) == 0x01000F0){ m_instruction = LDRSH; break; }
        if((m_op.val & 0xD700000) == 0x4300000){ m_instruction = LDRT; break; }
        if((m_op.val & 0xF100010) == 0xE000010){ m_instruction = MCR; break; }
        if((m_op.val & 0xFE000F0) == 0x200090 ){ m_instruction = MLA; break; }
        if((m_op.val & 0xF100010) == 0xE100010){ m_instruction = MRC; break; }
        if((m_op.val & 0xFBF0FFF) == 0x10F0000){ m_instruction = MRS; break; }
        if((m_op.val & 0xFB0F000) == 0x320F000){ m_instruction = MSR; break; }
        if((m_op.val & 0xFB0FFF0) == 0x120F000){ m_instruction = MSR; break; }
        if((m_op.val & 0xFE0F0F0) == 0x0000090){ m_instruction = MUL; break; }
        if((m_op.val & 0xDEF0000) == 0x1E00000){ m_instruction = MVN; break; }
        if((m_op.val & 0xFE000F0) == 0xE00090 ){ m_instruction = SMLAL; break; }
        if((m_op.val & 0xFE000F0) == 0xC00090 ){ m_instruction = SMULL; break; }
        if((m_op.val & 0xE100000) == 0xC00000 ){ m_instruction = STC; break; }
        if((m_op.val & 0xE500000) == 0x8000000){ m_instruction = STM_1; break; }
        if((m_op.val & 0xE700000) == 0x8400000){ m_instruction = STM_1; break; }
        if((m_op.val & 0xC500000) == 0x4000000){ m_instruction = STR; break; }
        if((m_op.val & 0xC500000) == 0x4400000){ m_instruction = STRB; break; }
        if((m_op.val & 0xD700000) == 0x4600000){ m_instruction = STRBT; break; }

        if((m_op.val & 0xD700000) == 0x4200000){ m_instruction = STRT; break; }

        if((m_op.val & 0xF000000) == 0xF000000){ m_instruction = SWI; break; }
        if((m_op.val & 0xFF00FF0) == 0x1000090){ m_instruction = SWP; break; }
        if((m_op.val & 0xFF00FF0) == 0x1400090){ m_instruction = SWPB; break; }
        if((m_op.val & 0xDF0F000) == 0x1300000){ m_instruction = TEQ; break; }
        if((m_op.val & 0xDF0F000) == 0x1100000){ m_instruction = TST; break; }
        if((m_op.val & 0xFE000F0) == 0xA00090 ){ m_instruction = UMLAL; break; }
        if((m_op.val & 0xFE000F0) == 0x800090 ){ m_instruction = UMULL; break; }
        /* TODO: Enhanced DSP Extensions */

        break;
    }

    /* Adressing Modes */
    switch( m_instruction )
    {
        case AND:
        case EOR:
        case SUB:
        case RSB:
        case ADD:
        case ADC:
        case SBC:
        case RSC:
        case CMP:
        case ORR:
        case MOV:
        case BIC:
            m_instruction |= _DATA;
            if( m_op.F.i ){ m_instruction |= It; break;}
            if( !(m_op.b12.val >> 4) ){ m_instruction |= Rt; break;}
            m_instruction |= (m_op.val & 0x10) ? RSt : ISt;
            break;
        case BR:
        case BL:
        case BX:
        case BLX:
            m_instruction |= _BRANCH;
            m_value = m_op.b24.val;
            if( m_value & 0x800000 ) m_value += 0xFF000000;
            m_value = (m_value << 2) + m_r[15] + 4;
            return;
        case LDR:
        case STR:
        case LDRB:
        case STRB:
            m_instruction |= _LOADSTORE;
            if( !m_op.F.i )
                m_instruction |= It;
            else
                m_instruction |= Rt;
            if( !m_op.F.p )
                m_instruction |= POSTt;
            else
            {
                if( !m_op.F.w )
                    m_instruction |= OFFSETt;
                else
                    m_instruction |= PREt;
            }
            if( !(m_op.b12.val >> 4) && m_op.F.i )
                m_instruction |= SCALEDt;
            break;
        case LDRT:
        case STRT:
        case LDRBT:
        case STRBT:
        case LDRH:
        case LDRSB:
        case LDRSH:
        case STRH:
            m_instruction |= _LOADSTORE;
            if( m_op.F.b )
            {    m_instruction |= It; /* FIXME: This does not work properly for miscellaneous load/store instructions */ return; }
            else
                m_instruction |= Rt;
            if( !m_op.F.p )
                m_instruction |= POSTt;
            else
            {
                if( !m_op.F.w )
                    m_instruction |= OFFSETt;
                else
                    m_instruction |= PREt;
            }
            break;
        case MSR:
        case MRS:
            if( m_op.F.i )
            {
                m_instruction |= It;
                m_value = rotater(imImd8b(m_op.val), imRotate(m_op.val) * 2);
            }
            else
            {
                m_instruction |= Rt;
                m_value = m_r[imRm(m_op.val)];
            }
            break;
    }

    if( !m_instruction ) return;

    /*FIXME: rename all the names that start with '_', its reserved for standard library functions */

    if( m_instruction & _DATA )
    {

        switch( m_instruction & DATA_MASK )
        {
            case It:
                m_value = rotater( m_op.b8.val, m_op.rotate.val * 2);
                if( m_op.rotate.val * 2 )
                    cpsr.bit.c = ( m_value & _COutBit && DCONLY == false ) ? 1 : 0;
                return;
            case Rt:
                m_value = m_r[m_op.R.m];
                if( m_op.R.m == 15 || m_op.R.n == 15 )
                    m_value = m_r[15] + 8;
                return;
            case RSt:
            case ISt:
                break;
        }
    }
    if( m_instruction & _LOADSTORE )
    {
    /* **************************************** *
     *	Instruction list:					    *
     *	LDR, STR, LDRB, STRB, LDRT, STRT,	    *
     *	LDRBT, STRBT, LDRH, LDRSB, LDRSH,	    *
     *	STRH.								    *
     *	Implemented:						    *
     *		LDR - YES(?)					    *
     *		STR - YES(?)					    *
     *		LDRB - NO						    *
     *		STRB - NO						    *
     *		LDRT - NO						    *
     *		STRT - NO						    *
     *		LDRBT - NO						    *
     *		STRBT - NO						    *
     *		LDRM - NO						    *
     *		LDRSB - NO						    *
     *		LDRSH - NO						    *
     *		STRH - YES(?)					    *
     * **************************************** */
        switch( m_instruction & LOADSTORE_MASK )
        {
            case IMMEDIATE_OFFSETt :
            case REGISTER_OFFSETt:
                m_adress = m_r[m_op.R.n];
                m_value = ( !m_op.F.i ) ? m_op.b12.val : m_r[m_op.R.m];
                m_adress += ( m_op.F.u ) ? m_value : -m_value;
                if( !m_op.F.i && m_op.R.n == 15 ) m_adress += 8;
                return;
            case SCALED_OFFSET_Rt:
                break;
            case IMMEDIATE_PREt:
            case REGISTER_PREt:
                m_value = ( m_op.F.i ) ? m_op.b12.val : m_r[m_op.R.m];
                m_adress += ( m_op.F.u ) ? m_value : -m_value;
                return;
            case SCALED_POST_Rt:
                m_adress = m_r[m_op.R.n];
            case SCALED_PRE_Rt:
                m_value = m_r[m_op.R.m];
                switch( m_op.shift.oper )
                {
                    case 0:
                        m_value = m_value << m_op.shift.amount;
                        break;;
                    case 1:
                        m_value = ( m_op.shift.amount ) ? (m_value >> m_op.shift.amount) : 0;
                        break;
                    case 2:
                        if( !m_op.shift.amount )
                            m_value = (m_value & SIGNBIT) ? 0xFFFFFFFF : 0;
                        else
                            m_value = (m_value & SIGNBIT) | ( m_value >> m_op.shift.amount);
                        break;
                    case 3:
                        m_value = ( m_op.shift.amount ) ? ((m_value >> 1) | (cpsr.bit.c << 31)) : rotater(m_value, m_op.shift.amount);
                }
                if( (m_instruction & LOADSTORE_MASK) == SCALED_PRE_Rt ){
                    m_adress = m_r[m_op.R.n];
                    m_adress += ( m_op.F.u ) ? m_value : -m_value;
                }

                return;
            case IMMEDIATE_POSTt:
                m_adress = m_r[m_op.R.n];
                if( DCONLY ) return;
                m_r[m_op.R.n] = ( iscond() ) ? ( (m_op.F.u) ? (m_r[m_op.R.n] + m_op.b12.val) : (m_r[m_op.R.n] - m_op.b12.val) ) : m_r[m_op.R.n] ;
            case REGISTER_POSTt:
                m_adress = m_r[m_op.R.n];
                if( DCONLY ) return;
                m_r[m_op.R.n] = ( iscond() ) ? ( (m_op.F.u) ? (m_r[m_op.R.n] + m_r[m_op.R.m]) : (m_r[m_op.R.n] - m_r[m_op.R.m]) ) : m_r[m_op.R.n];
                break;
        }
    }
}


void NDSCPU::thumb_decoder()
{

    thumbid.val = m_thumb_op.val;
    if( !thumbid.a.ident )
    {
        m_instruction |= ( !thumbid.a.opcode ) ? TLSL : TLSR;
        return;
    }

    if( thumbid.g.ident == TM_BRANCHEXCHANGE_ID ){ m_instruction |= TBX; return; }
    if( thumbid.p.ident_main == TM_MISC_PUSHPOPREGISTERLISTmain_ID && thumbid.p.ident_misc == TM_MISC_PUSHPOPREGISTERLISTmisc_ID )
    {
        m_instruction |= ( m_thumb_op.F.l_sp ) ? TPOP : TPUSH;
        return;
    }
    /* BRANCH */
    if( thumbid.u.ident == TM_BLXPREFIX_ID )
    {
        union THUMB_OPCODE_BREAKDOWN suffix;
        m_value = m_r[15] + 2;
        suffix.val = mem->read_word( m_value );
        if( suffix.u.ident == TM_BLXSUFFIX_ID )
        {
            m_instruction |= TBLX;
        }else{
            m_instruction |= TBL;
        }
        union THUMB_OPCODE_VALUES Suffix;
        Suffix.val = mem->read_hword( m_value );
        m_adress = ( m_thumb_op.offset.val << 12 ) + 2;
        m_adress += Suffix.offset.val * 2;
        m_adress += m_r[15];

        return;
    }

    /* ADD Subtract Move or Compare 8bit immediate */
    if( thumbid.d.ident == TM_ADDSUBTRACT_COMPARE_MOVEi_ID )
    {
        m_value = m_thumb_op.b8.a;
        switch( thumbid.d.opcode )
        {
            case 0:
                m_instruction |= TMOV;
                m_op.R.d = m_thumb_op.R.nd;
                m_adress = 1;
                return;
            case 1:
                m_instruction |= TCMP | ( It );
                m_op.R.d = m_thumb_op.b8.b & 7;
                m_value = m_thumb_op.b8.a;
                return;
            case 2:
                m_instruction |= TADD;
                m_op.R.d = m_thumb_op.R.nd;
                return;
            case 3:
                m_instruction |= TSUB;
                m_op.R.d = m_thumb_op.R.nd;
                return;
        }
    }
    if( thumbid.c.ident == TM_ADDSUBTRACTi_ID )
    {
        switch( thumbid.c.opc )
        {
            case 0:
                if( !m_thumb_op.R.m )
                {
                    m_instruction |= TMOV;
                    m_value = m_r[m_thumb_op.R.nm];
                    m_op.R.d = m_thumb_op.R.dn;
                    m_adress = 1;
                    m_op.F.s = 1;
                    return;
                }else{
                    m_instruction |= TADD;
                    return;
                }
                return;
            case 1:
                m_instruction |= TSUB;
                return;
        }
    }
    if( thumbid.f.ident == TM_SPECIALDATAPROCESSING_ID )
    {
        switch ( thumbid.f.opcode )
        {
            case 0:
            case 1:
                return;
            case 2:
                m_instruction |= TMOV;
                m_op.R.m = ( m_thumb_op.F.h2 << 3 ) + m_thumb_op.R.nm;
                m_value = m_r[m_op.R.m];
                m_op.R.d = ( m_thumb_op.F.h1 << 3 ) + m_thumb_op.R.dn;
                m_adress = 0;
                return;
        }
    }

    /* LOAD STORE */
    if( thumbid.h.ident == TM_LOADFROMLITERALPOOL_ID )
    {
        m_instruction |= ( TLDR | SCALED_OFFSET_Rt );
        m_value = ( m_thumb_op.val & 0xFF) << 2;
        m_op.R.d = (m_thumb_op.val >> 8) & 7;
        m_adress = m_value + ( m_r[15] & 0xFFFFFFFC ) + 4;
        return;
    }
    if( thumbid.i.ident == TM_LOADSTOREroff_ID )
    {
        switch( thumbid.i.opcode )
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                return;
            case 8:
                m_instruction |= ( TLDR | Rt | OFFSETt );
                m_op.R.m = m_thumb_op.R.m;
                m_op.R.n = m_thumb_op.R.nm;
                m_op.R.d = m_thumb_op.R.dn;
                m_adress = m_r[m_op.R.n] + m_r[m_op.R.m];

                return;
        }
    }
    if( thumbid.j.ident == TM_LOADSTOREWORDBYTEioff_ID )
    {
        if( m_thumb_op.F.l_sp )
        {
            if( !m_thumb_op.F.b )
            {
                m_instruction |= ( TLDR | It | OFFSETt );
                m_value = (m_thumb_op.val >> 5) & 0x1F;
                m_op.R.m = m_thumb_op.R.nm;
                m_op.R.d = m_thumb_op.R.dn;
                m_adress = m_r[m_op.R.m] + (m_value * 4);
                return;
            }
        }
    }
    if( thumbid.k.ident == TM_LOADSTORESTACK_ID )
    {
        m_instruction |= ( TLDR | REGISTER_POSTt );
        m_value = m_thumb_op.b8.a * 4;
        m_op.R.d = (m_thumb_op.val >> 8) & 7;
        m_adress = m_value + ( m_r[13] );
        return;
    }
    if( thumbid.s.ident == TM_CONDITIONALBRANCH_ID )
    {
        m_instruction |= ( TBR | OFFSETt );
        m_op.cond.val = m_thumb_op.b4.c;
        m_adress = ((s32)((s8)(m_thumb_op.b8.a)))<<1;
        m_adress += m_r[15] + 2;
        return;
    }

}
