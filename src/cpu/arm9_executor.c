#include "cpu.h"
#include "opcodes.h"

static void arm9_modeswitch(ndsCPU *cpu, u32 newmode)
{

}

void arm9_executor(ndsCPU *cpu)
{
    u32 result = 0;
    u8 r8; // 8-bit result
    u16 r16; // 16-bit result
    u64 ur64 = 0;

    switch( cpu->instruction )
    {
        case AND :
            cpu->result.dword = RN & cpu->operand;
            cpu->s_type = SET_N_Z_OC;
            return;
        case EOR :
            cpu->result.dword = RN ^ cpu->operand;
            cpu->s_type = SET_N_Z_OC;
            return;
        case SUB :
            cpu->result.dword = RN - cpu->operand;
            cpu->s_type = SET_N_Z_RC_V;
            return;
        case RSB :
            cpu->result.dword = cpu->operand - RN;
            cpu->s_type = SET_N_Z_RC_RV;
            return;
        case ADD :
            cpu->result.dword = RN + cpu->operand;
            cpu->s_type = SET_N_Z_C_V;
            return;
        case ADC :
            cpu->result.dword = RN + cpu->operand + cpu->status->C;
            cpu->s_type = SET_N_Z_C_V;
            return;
        case SBC :
            cpu->result.dword = RN + cpu->operand + cpu->status->C;
            cpu->s_type = SET_N_Z_C_V;
            return;
        case RSC :
            cpu->result.dword = cpu->operand - RN - cpu->status->C;
            cpu->s_type = SET_N_Z_RC_V;
            return;
        case TST :
            cpu->result.dword = RN & cpu->operand;
            cpu->s_type = SET_N_Z_C;
            return;
        case TEQ :
            cpu->result.dword = RN ^ cpu->operand;
            cpu->s_type = SET_N_Z_C;
            return;
        case CMP :
            cpu->result.dword = RN - cpu->operand;
            cpu->s_type = SET_N_Z_RC_V;
            return;
        case CMN :
            cpu->result.dword = RN + cpu->operand;
            cpu->s_type = SET_N_Z_C_V;
            return;
        case ORR :
            cpu->result.dword = RN | cpu->operand;
            cpu->s_type = SET_N_Z_C;
            return;
        case MOV :
            cpu->result.dword = cpu->operand;
            cpu->s_type = SET_N_Z_C;
            return;
        case BIC :
            cpu->result.dword = RN & (~cpu->operand);
            cpu->s_type = SET_N_Z_C;
            return;
        case MVN :
            cpu->result.dword = ~cpu->operand;
            cpu->s_type = SET_N_Z_C;
            return;
        case MUL :
            cpu->result.dword = (RM * RS);
            cpu->s_type = SET_N_Z;
            return;
        case MLA :
            cpu->result.dword = RM * RS + RN;
            cpu->s_type = SET_N_Z;
            return;
        case SMULL :
            cpu->result.dword = (signed)RM * (signed)RS;
            //RN = ((sr64) & 0xFFFFFFFF00000000) >> 32;
            //RD = ((sr64) & 0x00000000FFFFFFFF);
            cpu->s_type = SET_N_Z;
            return;
        case UMULL :
            cpu->result.dword = RM * RS;
            //RN = (ur64 & 0xFFFFFFFF00000000) >> 32;
            //RD = (sr64 & 0x00000000FFFFFFFF);
            cpu->s_type = SET_N_Z;
            return;
        case SMLAL :
            cpu->result.dword = (signed)(RM * RS);
            //RD = (sr64 & 0x00000000FFFFFFFF) + RD;
            //RN = ((sr64 & 0xFFFFFFFF00000000) >> 32) + RN + SET_CARRY(RM, RS, sr64 & 0x00000000FFFFFFFF );
            cpu->s_type = SET64_N_Z;
            return;
        case UMLAL :
            cpu->result.dword = (signed)(RM * RS);
            //RD = (ur64 & 0x00000000FFFFFFFF) + RD;
            //RN = ((ur64 & 0xFFFFFFFF00000000) >> 32) + RN + SET_CARRY(RM, RS, ur64 & 0x00000000FFFFFFFF );
            cpu->s_type = SET64_N_Z;
            return;
        case BL :
            LR = PC + 4;
        case BR :
            cpu->newpc = (cpu->adress & 0xFFFFFFFD) + 4;
            return;
        case BLX :
            LR = PC + 4;
        case BX :
            cpu->newpc = cpu->adress & 0xFFFFFFFE;
            cpu->status->T = cpu->adress & 1;
            return;
        case CLZ :
            if( !RM ){
                RD = 32;
            } else {
                for( ur64 = 31; ur64 > 0; ur64-- ){
                    RD = 31 - ur64;
                    if( RM & (1 << ur64) )
                        break;
                }
            }
            return;
        case MRS :
            RD = ( cpu->op.B ) ? cpu->status->value : cpu->saved_status->value;
            return;
        case MSR :{
            arm9CPSR *swap = (cpu->op.OP_MSR.R) ? cpu->status : cpu->saved_status;
            if( cpu->op.I ){
                result = ROR(cpu->op.DATAI.immed_8,cpu->op.DATAI.rotate_imm);
            } else {
                result = RM;
            }
            if( cpu->op.OP_MSR.field_mask == 0xF ){
                swap->value = result;
                return;
            } else {
                if( cpu->op.OP_MSR.field_mask & 0x01){
                    /* Control Field */
                    swap->value |= (result & 0xFF);
                } else if( cpu->op.OP_MSR.field_mask & 0x02){
                    /* Extension Field */
                    swap->value |= (result & 0xFF00);
                } else if( cpu->op.OP_MSR.field_mask & 0x04){
                    /* Status Field */
                    swap->value |= (result & 0xFF0000);
                } else if( cpu->op.OP_MSR.field_mask & 0x08){
                    /* Flags Field */
                    swap->value |= (result & 0xFF000000);
                } 
            }
            break;
        }
        case LDR :
            result = read_word(cpu->adress, cpu->memory);
            result = ROR(result, (cpu->adress & 0x3)*8);
            RD = result;
            if( cpu->op.Rd == 15 ){
                PC = result & 0xFFFFFFFE;
                cpu->status->T = BITSET(result, 0);
            }
            break;
        case LDRB :
        case LDRBT :
            RD = (u32)read_byte(cpu->adress, cpu->memory);
            break;
        case LDRT :
            result = read_word(cpu->adress, cpu->memory);
            RD = ROR(result, (cpu->adress & 0x3) * 8);
            return;
        case STR :
        case STRT :
            write_word(cpu->adress, RD, cpu->memory);
            return;
        case STRB :
        case STRBT :
            write_byte(cpu->adress, (RD & 0xFF), cpu->memory);
            return;
        case STRH:
            write_halfword(cpu->adress, (RD & 0xFFFF), cpu->memory);
            return;
        case LDRH:
            RD = read_halfword(cpu->adress, cpu->memory);
            return;
        case LDRSB:
        case LDRSH:
            result = (u32)read_byte(cpu->adress, cpu->memory);
            RD = (result ^ 0xFFFFFFFF) - 0xFFFFFFFF; // sign-extend to 32 bits;
            return;
        //FIXME : case LDRBH:
        case STRD:
            if( !(RD & 0x01) ){
                if( cpu->op.Rd != 14 ){
                    write_word(cpu->adress, cpu->R[cpu->op.Rd], cpu->memory);
                    write_word(cpu->adress, cpu->R[cpu->op.Rd+1], cpu->memory);
                    return;
                }
            }
        case LDRD:
            if( !(RD & 0x01) ){
                if( cpu->op.Rd != 14 ){
                    cpu->R[cpu->op.Rd] = read_word(cpu->adress, cpu->memory);
                    cpu->R[cpu->op.Rd+1] = read_word(cpu->adress, cpu->memory);
                    return;
                }
            }
        case LDM_1 :{
            /* Set offset */
            int i = (cpu->op.U) ? 4 : -4;
            /* Set start adress */
            cpu->adress = RN - i + (cpu->op.P) ? i : 0;
            /* Set bit-set count */
            u32 count = ((cpu->op.value & 0x3FFF) * 0x200040008001ULL & 0x1111111111111111ULL) % 0xF;
            if( BITSET(cpu->op.value, 14) ) /* LR */
                count += 1;
            if( BITSET(cpu->op.value, 15) ) /* PC */
                count += 1;
			/* Unrolled loop */
            if(B0(cpu->op.value)) cpu->R[0] = read_word(cpu->adress+=i,cpu->memory);
            if(B1(cpu->op.value)) cpu->R[1] = read_word(cpu->adress+=i, cpu->memory);
            if(B2(cpu->op.value)) cpu->R[2] = read_word(cpu->adress+=i, cpu->memory);
            if(B3(cpu->op.value)) cpu->R[3] = read_word(cpu->adress+=i, cpu->memory);
            if(B4(cpu->op.value)) cpu->R[4] = read_word(cpu->adress+=i, cpu->memory);
            if(B5(cpu->op.value)) cpu->R[5] = read_word(cpu->adress+=i, cpu->memory);
            if(B6(cpu->op.value)) cpu->R[6] = read_word(cpu->adress+=i, cpu->memory);
            if(B7(cpu->op.value)) cpu->R[7] = read_word(cpu->adress+=i, cpu->memory);
            if(B8(cpu->op.value)) cpu->R[8] = read_word(cpu->adress+=i, cpu->memory);
            if(B9(cpu->op.value)) cpu->R[9] = read_word(cpu->adress+=i, cpu->memory);
            if(B10(cpu->op.value)) cpu->R[10] = read_word(cpu->adress+=i, cpu->memory);
            if(B11(cpu->op.value)) cpu->R[11] = read_word(cpu->adress+=i, cpu->memory);
            if(B12(cpu->op.value)) cpu->R[12] = read_word(cpu->adress+=i, cpu->memory);
            if(B13(cpu->op.value)) cpu->R[13] = read_word(cpu->adress+=i, cpu->memory);
            if(B14(cpu->op.value)) cpu->R[14] = read_word(cpu->adress+=i, cpu->memory);

            if(B15(cpu->op.value)){
                PC = read_word(cpu->adress+=i, cpu->memory);
                cpu->status->T = PC & 0x01;
                PC &= 0xFFFFFFFE;
            }
			
			if( cpu->op.W ){
				RN += count * 4;
			}

            return;
        }
        case LDM_2 :
        case LDM_3 :
            break;
        case STM_1 :{
            int i = (cpu->op.U) ? 4 : -4;
			cpu->adress = RN - i + (cpu->op.P) ? 1 : 0;
			u32 count = ((cpu->op.value & 0x3FFF) * 0x200040008001ULL & 0x1111111111111111ULL) % 0xF;
			if( BITSET(cpu->op.value, 14) ) /* LR */
				count += 1;
			if( BITSET(cpu->op.value, 15) ) /* PC */
				count += 1;
	        if(B0(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[0], cpu->memory);
            if(B1(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[1], cpu->memory);
            if(B2(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[2], cpu->memory);
            if(B3(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[3], cpu->memory);
            if(B4(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[4], cpu->memory);
            if(B5(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[5], cpu->memory);
            if(B6(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[6], cpu->memory);
            if(B7(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[7], cpu->memory);
            if(B8(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[8], cpu->memory);
            if(B9(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[9], cpu->memory);
            if(B10(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[10], cpu->memory);
            if(B11(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[11], cpu->memory);
            if(B12(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[12], cpu->memory);
            if(B13(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[13], cpu->memory); 
            if(B14(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[14], cpu->memory);
			if(B15(cpu->op.value)) write_word(cpu->adress+=i, cpu->R[15], cpu->memory);
			
			if( cpu->op.W ){
				RN += count * 4;
			}

			return;
        }
        case STM_2 :
            break;
        /* Semaphore Instructions */
        case SWP :
            result = read_word(cpu->adress, cpu->memory);
            result = ROR(result, (cpu->adress & 0x03) * 8);
            RD ^= result;
            result ^= RD;
            RD ^= result;
            write_word(cpu->adress, result, cpu->memory);
            return;
        case SWPB :
            result = (u32)read_byte(RN, cpu->memory);
            write_byte(RN, (u8)(RN & 0xFF), cpu->memory);
            RD = result;
            return;
        /* Exception-Generating Instructions */
        case BKPT :
            /* TODO BREAKPOINT INSTRUCTION */
            return;
        case SWI :
            /* TODO SWI */
            return;
        /* Coprocessor Instructions */
        case CDP :
            return;
        case LDC :
            return;
        case MCR :
        case MRC :
            return;
        case STC :
            return;
        /* Extended Instructons */
        case QADD :
        case QSUB :
        case QDADD :
        case QDSUB :
        case SMLAXY :
        case SMLAWY :
        case SMULWY :
        case SMLALXY :
        case SMULXY :
        case PLD :
        case STC2 :
        case LDC2 :
        case CDP2 :
        case MCR2 :
        case MRC2 :
            break;
    }
}
