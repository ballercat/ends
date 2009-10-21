#include "cpu.h"
#include "opcodes.h"
#include<stdlib.h>
#include<string.h>



void arm9_decoder(struct ndsCPU *cpu){
    u32 tmp;
    u8 tmp8;
    while( !cpu->instruction )
    {
        /* Regular arm9 Instructions */
        if((cpu->op.value & 0xE1000F0) == 0x00000B0){ cpu->instruction = STRH; break; }
        switch(cpu->op.value & 0xDE00000)
        {
            case 0x0000000: cpu->instruction = AND; break;
            case 0x0200000: cpu->instruction = EOR; break;
            case 0x0400000: cpu->instruction = SUB; break;
            case 0x0600000: cpu->instruction = RSB; break;
            case 0x0800000: cpu->instruction = ADD; break;
            case 0x0A00000: cpu->instruction = ADC; break;
            case 0x0C00000: cpu->instruction = SBC; break;
            case 0x0E00000: cpu->instruction = RSC; break;
            case 0x1400000: cpu->instruction = CMP; break;
            case 0x1800000: cpu->instruction = ORR; break;
            case 0x1A00000: cpu->instruction = MOV; break;
            case 0x1C00000: cpu->instruction = BIC; break;
        }
        if( cpu->instruction ) break;
        if((cpu->op.value & 0xC500000) == 0x4100000){ cpu->instruction = LDR; break; }
        if((cpu->op.value & 0xC500000) == 0x4500000){ cpu->instruction = LDRB; break; }
        if((cpu->op.value & 0xD700000) == 0x6700000){ cpu->instruction = LDRBT; break; }

        if((cpu->op.value & 0xF000000) == 0xA000000){ cpu->instruction = BR;  break; }
        if((cpu->op.value & 0xF000000) == 0xB000000){ cpu->instruction = BL;  break; }
        if((cpu->op.value & 0xFFF000F0)==0xE1A00070){ cpu->instruction = BKPT; break; }
        if((cpu->op.value & 0xFE000000)==0xFA000000){ cpu->instruction = BLX; break; }
        if((cpu->op.value & 0xFFFFFF0) == 0x12FFF30){ cpu->instruction = BLX; break; }
        if((cpu->op.value & 0xFFFFFF0) == 0x12FFF10){ cpu->instruction = BX;  break; }
        if((cpu->op.value & 0xF000010) == 0xE000000){ cpu->instruction = CDP; break; }
        if((cpu->op.value & 0xFFF0FF0) == 0x16F0F10){ cpu->instruction = CLZ; break; }
        if((cpu->op.value & 0xDF0F000) == 0x3700000){ cpu->instruction = CMN; break; }
        if((cpu->op.value & 0xE100000) == 0xC100000){ cpu->instruction = LDC; break; }
        if((cpu->op.value & 0xE500000) == 0x8100000){ cpu->instruction = LDM_1; break; }
        if((cpu->op.value & 0xE708000) == 0x8500000){ cpu->instruction = LDM_2; break; }
        if((cpu->op.value & 0xE508000) == 0x8508000){ cpu->instruction = LDM_3; break; }

        if((cpu->op.value & 0xE1000F0) == 0x01000B0){ cpu->instruction = LDRH; break; }
        if((cpu->op.value & 0xE1000F0) == 0x01000D0){ cpu->instruction = LDRSB; break; }
        if((cpu->op.value & 0xE1000F0) == 0x01000F0){ cpu->instruction = LDRSH; break; }
        if((cpu->op.value & 0xD700000) == 0x4300000){ cpu->instruction = LDRT; break; }
        if((cpu->op.value & 0xF100010) == 0xE000010){ cpu->instruction = MCR; break; }
        if((cpu->op.value & 0xFE000F0) == 0x200090 ){ cpu->instruction = MLA; break; }
        if((cpu->op.value & 0xF100010) == 0xE100010){ cpu->instruction = MRC; break; }
        if((cpu->op.value & 0xFBF0FFF) == 0x10F0000){ cpu->instruction = MRS; break; }
        if((cpu->op.value & 0xFB0F000) == 0x320F000){ cpu->instruction = MSR; break; }
        if((cpu->op.value & 0xFB0FFF0) == 0x120F000){ cpu->instruction = MSR; break; }
        if((cpu->op.value & 0xFE0F0F0) == 0x0000090){ cpu->instruction = MUL; break; }
        if((cpu->op.value & 0xDEF0000) == 0x1E00000){ cpu->instruction = MVN; break; }
        if((cpu->op.value & 0xFE000F0) == 0xE00090 ){ cpu->instruction = SMLAL; break; }
        if((cpu->op.value & 0xFE000F0) == 0xC00090 ){ cpu->instruction = SMULL; break; }
        if((cpu->op.value & 0xE100000) == 0xC00000 ){ cpu->instruction = STC; break; }
        if((cpu->op.value & 0xE500000) == 0x8000000){ cpu->instruction = STM_1; break; }
        if((cpu->op.value & 0xE700000) == 0x8400000){ cpu->instruction = STM_1; break; }
        if((cpu->op.value & 0xC500000) == 0x4000000){ cpu->instruction = STR; break; }
        if((cpu->op.value & 0xC500000) == 0x4400000){ cpu->instruction = STRB; break; }
        if((cpu->op.value & 0xD700000) == 0x4600000){ cpu->instruction = STRBT; break; }

        if((cpu->op.value & 0xD700000) == 0x4200000){ cpu->instruction = STRT; break; }

        if((cpu->op.value & 0xF000000) == 0xF000000){ cpu->instruction = SWI; break; }
        if((cpu->op.value & 0xFF00FF0) == 0x1000090){ cpu->instruction = SWP; break; }
        if((cpu->op.value & 0xFF00FF0) == 0x1400090){ cpu->instruction = SWPB; break; }
        if((cpu->op.value & 0xDF0F000) == 0x1300000){ cpu->instruction = TEQ; break; }
        if((cpu->op.value & 0xDF0F000) == 0x1100000){ cpu->instruction = TST; break; }
        if((cpu->op.value & 0xFE000F0) == 0xA00090 ){ cpu->instruction = UMLAL; break; }
        if((cpu->op.value & 0xFE000F0) == 0x800090 ){ cpu->instruction = UMULL; break; }
        /* TODO: Enhanced DSP Extensions */

        break;
    }
    if( cpu->instruction ){

    }
    switch( cpu->instruction )
    {
        case AND :
        case EOR :
        case SUB :
        case RSB :
        case ADD :
        case ADC :
        case SBC :
        case RSC :
        case TST :
        case TEQ :
        case CMP :
        case CMN :
        case ORR :
        case MOV :
        case BIC :
        case MVN :
            /* Check S */
            if( cpu->op.S ){
                cpu->set_flags = TRUE;
            }
            /* Adressing Mode 1 - Data-processing operands */
            if( cpu->op.F.i ){ /* Immediate */
                cpu->adressing = AD_DATA_IMMEDIATE;
				cpu->operand = ROR(cpu->op.DATAI.immed_8, cpu->op.DATAI.rotate_imm*2);
                if( cpu->op.DATAI.rotate_imm == 0 ){
                    cpu->carry = cpu->cpsr.C;
                } else {
                    cpu->carry = B31(cpu->operand);
                }
                break;
            } else { /* Immediate Shift, Register Shift */
                if(!B4(cpu->op.value)){
                    /* Immediate Shifts */
                    if( !(cpu->op.value & 0xFF0)){ /* Data-processing operands-Register */
                        cpu->adressing = AD_DATA_REGISTER;
						cpu->operand = RM;
                        cpu->carry = cpu->cpsr.C;
                        break;
                    }

                    switch( cpu->op.DATAIS.shift ){
                    case LSLVALUE: /* Data-processing operands - Logical shift left by immediate */
						cpu->adressing = AD_DATA_LSL_IMMEDIATE;
                        cpu->operand = RM << cpu->op.DATAIS.shift;
                        cpu->carry = BITSET(RM, 32 - cpu->op.DATAIS.shift );
                        break;
                    case LSRVALUE: /* Data-processing operands - Logical shift right by immediate */
                        cpu->adressing = AD_DATA_LSR_IMMEDIATE;
						if(cpu->op.DATAIS.shift == 0){
                            cpu->operand = 0;
                            cpu->carry = B31(RM);
                        } else {
                            cpu->operand = RM >> cpu->op.DATAIS.shift;
                            cpu->carry = BITSET(RM, cpu->op.DATAIS.shift - 1);
                        }
                        break;
                    case ASRVALUE: /* Data-processing operands - Arithmetic shift right by immediate */
						cpu->adressing = AD_DATA_ASR_IMMEDIATE;
                        if( cpu->op.DATAIS.shift == 0 ){
                            if(!B31(RM)){
                                cpu->operand = 0;
                                cpu->carry = 0;
                            } else {
                                cpu->operand = 0xFFFFFFFF;
                                cpu->carry = 1;
                            }
                        } else {
                            cpu->operand = (signed)RM >> cpu->op.DATAIS.shift;
                            cpu->carry = BITSET(RM, cpu->op.DATAIS.shift - 1);
                        }
                        break;
                    case RORVALUE: /* Data-processing operands - Rotate right by immediate */
                        if( cpu->op.DATAIS.shift == 0 ){
							cpu->adressing = AD_DATA_RRX_IMMEDIATE;
                            /* Data-processing operands - Rotate right with extend */
                            cpu->operand = RRX(cpu->cpsr.C, RM);
                            cpu->carry = BITSET(RM, 0);
                        } else {
							cpu->adressing = AD_DATA_ROR_IMMEDIATE;
                            cpu->operand = ROR(RM, cpu->op.DATAIS.shift );
                            cpu->carry = BITSET(RM, cpu->op.DATAIS.shift - 1);
                        }
                    }
                    cpu->operand += (RM == 15 || RN == 15) ? 8 : 0;
                    break;
                } else {
                    /* Register Shifts */
                    if( RD == 15 || RM == 15 || RN == 15 || RS == 15 ){
                        break;
                    }
                    tmp = cpu->op.value & 0xFF;
                    switch(cpu->op.DATARS.shift){
                    case LSLVALUE: /* Data-processing operands-Logical shift left by register */
						cpu->adressing = AD_DATA_LSL_REGISTER;
                        cpu->operand = RM << tmp;
                        if( tmp && tmp < 32 ){
                            cpu->carry = BITSET(RM, 32 - tmp);
                        } else if( tmp == 32 ){
                            cpu->carry = BITSET(RM, 0);
                        }
                        break;
                    case LSRVALUE: /* Data-processing operands-Logical shift right by register */
						cpu->adressing = AD_DATA_LSR_REGISTER;
                       cpu->operand = RM >> tmp;
                       if( tmp && tmp < 32 ){
                           cpu->carry = BITSET(RM, tmp - 1);
                       } else if( tmp == 32 ){
                           cpu->carry = BITSET(RM, 31);
                       }
                       break;
                    case ASRVALUE: /* Data-processing operands-Arithmetic shift right by register */
						cpu->adressing = AD_DATA_ASR_REGISTER;
                        cpu->operand = ASR(RM, tmp);
                        if( tmp && tmp < 32 ){
                            cpu->carry = BITSET(RM, tmp -1);
                        } else if( tmp >= 32 ){
                            if( BITSET(RM, 31) ){
                                cpu->operand = 0xFFFFFFFF;
                                cpu->carry = BITSET(RM, 31);
                            } else {
                                cpu->operand = 0;
                                cpu->carry = BITSET(RM, 31);
                            }
                        }
                    case RORVALUE: /* Data-processing operands-Rotate right by register */
						cpu->adressing = AD_DATA_ROR_REGISTER;
                        cpu->operand = ROR(RM, tmp & 0xF);
                        if( tmp == 0 ){
                            cpu->operand = RM;
                        } else if( !(tmp & 0xF) ){
                            cpu->operand = RM;
                            cpu->carry = BITSET(RM, 31);
                        } else if( (tmp & 0xF) > 0 ){
                            cpu->carry = BITSET(RM, (tmp&0xF) - 1);
                        }
                    }
                    break;
                }
                
            }
        case MUL :
        case MLA :
        case SMULL :
        case UMULL :
        case SMLAL :
        case UMLAL :
            if( cpu->op.S ){
                cpu->set_flags = TRUE;
            }
            break;
        case BR :
        case BL :
            cpu->adress = cpu->op.b24.val;
            if( cpu->adress & 0x800000 ){
                cpu->adress += 0xFF000000;
            }
            cpu->adress = (cpu->adress << 2) + cpu->R[15] + 4;
            break;
        case BLX :
        case BX :
            cpu->adress = RM;
            cpu->cpsr.T = cpu->adress & 0x01;
            break;
        case CLZ  :
        case MRS :
        case MSR :
            break;
        case LDR :
        case LDRB :
        case LDRBT :
        case LDRT :
        case STR :
        case STRB :
        case STRBT :
        case STRT :
            /* Adressing Mode 2 - Load and Store Word or Unsigned Byte */
            if( cpu->op.F.i == 0 ){
                /* Immediate offset/index */
                if( cpu->op.F.p && !cpu->op.F.w ){
                    /* Load and Store Word or Unsigned Byte-Immediate offset */
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.F.u) ? cpu->op.b12.val : -cpu->op.b12.val;
                    cpu->adress += (RN == 15) ? 8 : 0;
                    break;
                } else if( cpu->op.F.p && cpu->op.F.w && RN != 15){
                    /* Load and Store Word or Unsigned Byte-Immediate pre-indexed */
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.F.u) ? cpu->op.b12.val : -cpu->op.b12.val;
                    cpu->rn = cpu->adress;
                    break;
                } else {
                    if( RN == 15 ){
                        /* TODO: log an error */
                        break;
                    }
                    cpu->adress = RN;
                    cpu->rn = RN + ((cpu->op.F.u) ? cpu->op.b12.val : -cpu->op.b12.val);
                    break;
                }
            } else if( !(cpu->op.value & 0xFF0) ){ /* Register offset/index */
                if( cpu->op.F.p && !cpu->op.F.w ){
                    /* Load and Store Word or Unsigned Byte-Register offset */
                    if( RM == 15 ){
                        /* TODO : log an error */
                        break;
                    }
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.F.u) ? RM : -RM;
                    cpu->adress += (RN==15) ? 8 : 0;
                    break;
                } else if( cpu->op.F.p && cpu->op.F.w ){
                    /* Load and Store Word or Unsigned Byte-Register pre-indexed */
                    if( RN == 15 || RM == 15 ){
                        /* TODO : log an error */
                        break;
                    }
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.F.u) ? RM : -RM;
                    cpu->rn = cpu->adress;
                    break;
                } else {
                    /* Load and Store Word or Unsigned Byte-Register post-indexed */
                    if( RN == 15 || RM == 15 ){
                        /* TODO : log an error */
                        break;
                    }
                    cpu->adress = RN;
                    cpu->rn = RN + ( (cpu->op.F.u) ? RM : -RM );
                    break;
                }
            } else if( !(cpu->op.value & 0x10) ){ /* Scaled register offset/index */
                switch(cpu->op.shift.oper){
                case LSLVALUE:
                    tmp = RM << cpu->op.shift.amount;
                    break;
                case LSRVALUE:
                    tmp = (cpu->op.shift.amount) ? (RM >> cpu->op.shift.amount) : 0;
                    break;
                case ASRVALUE:
                    if( cpu->op.shift.amount ){
                        tmp = (BITSET(RM, 31)) ? 0xFFFFFFFF : 0;
                    } else {
                        tmp = ASR(RM, cpu->op.shift.amount);
                    }
                    break;
                case RORVALUE:
                    if( cpu->op.shift.amount ){
                        tmp = ROR(RM, cpu->op.shift.amount);
                    } else {
                        ;tmp = RRX(cpu->cpsr.C,RM);
                    }
                    break;
                }

                if( cpu->op.F.p && !cpu->op.F.w ){
                    /* Load and Store Word or Unsigned Byte - Scaled register offset */
                    if( RM == 15){
                        /* TODO : log an error */
                        break;
                    }
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.F.u) ? tmp : -tmp;
                    cpu->adress += (RN == 15) ? 8 : 0;
                    break;
                } else if( cpu->op.F.p && cpu->op.F.w ){
                    /* Load and Store Word or Unsigned Byte - Scaled register pre-indexed */
                    if( RM == 15 || RN == 15 ){
                        /* TODO : log an error */
                        break;
                    }
                    cpu->adress = RN + ((cpu->op.F.u) ? tmp : -tmp);
                    cpu->rn = cpu->adress;
                } else {
                    /* Load and Store Word or Unsigned Byte - Scaled register post-indexed */
                    cpu->adress = RN;
                    cpu->rn = RN + ((cpu->op.F.u) ? tmp : -tmp);
                    break;
                }
            }
            break;
        case STRH:
        case LDRH:
        case LDRSB:
        case LDRSH:
        case LDRBH:
        case STRD:
        case LDRD:
            /* Adressing Mode 3 - Miscellaneous Loads and Stores */
            if( cpu->op.B ){
                /* Immediate offset/index */
                if( cpu->op.P && !cpu->op.W ){ /* Miscellaneous Loads and Stores - Immediate offset */
                    tmp8 = (cpu->op.Rs << 4) | cpu->op.Rm;
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.U) ? tmp8 : -tmp8;
                    break;
                } else if( cpu->op.P && cpu->op.W ){ /* Miscellaneous Loads and Stores - Immediate pre-indexed */
                    tmp8 = (cpu->op.Rs << 4) | cpu->op.Rm;
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.U) ? tmp8 : -tmp8;
                    cpu->rn = cpu->adress;
                    break;
                } else { /* Miscellaneous Loads and Stores - Immediate post-indexed */
                    cpu->adress = RN;
                    tmp8 = (cpu->op.Rs << 4) | cpu->op.Rm;
                    cpu->rn = RN + (cpu->op.U) ? tmp8 : -tmp8;
                    break;
                }
            } else {
                /* Register offset/index */
                if( cpu->op.P && !cpu->op.W ){
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.U) ? RM : -RM;
                    break;
                } else if( cpu->op.P && cpu->op.W ){
                    cpu->adress = RN;
                    cpu->adress += (cpu->op.U) ? RM : -RM;
                    cpu->rn = cpu->adress;
                    break;
                } else {
                    cpu->adress = RN;
                    cpu->rn = RN + (cpu->op.U) ? RM : -RM;
                    break;
                }
            }
            break;
        case LDM_1 :
        case LDM_2 :
        case LDM_3 :
        case STM_1 :
        case STM_2 :
        /* Semaphore Instructions */
        case SWP :
        case SWPB :
        /* Exception-Generating Instructions */
        case BKPT :
        case SWI :
        /* Coprocessor Instructions */
        case CDP :
        case LDC :
        case MCR :
        case MRC :
        case STC :
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


