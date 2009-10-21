#include "cpu.h"
#include "opcodes.h"
#include<stdlib.h>
#include<string.h>

static u32 condition_mask[] =   {0x40000000, 0x40000000, 0x20000000, 0x20000000,
                                0x80000000, 0x80000000, 0x10000000, 0x10000000,
                                0x60000000, 0x60000000 };

static u32 condition_result[] = {0x40000000, 0x00000000, 0x20000000, 0x00000000,
                                0x80000000, 0x00000000, 0x10000000, 0x00000000,
                                0x20000000, 0x60000000 };

static void arm9_execute(ndsCPU *cpu);
static u8 condition(ndsCPU *cpu); /* BOOLEAN RETURN */

/* Create a new arm9 cpu */
struct ndsCPU *create_cpu_arm9()
{
    u32 k;
    struct ndsCPU *arm9 = malloc( sizeof(*arm9) );
    if( arm9 ){

		arm9->exec = &arm9_execute;

        for( k = 0; k < 0x10; k++ ){
                arm9->R[k] = 0;
        }
		arm9->pc = &arm9->R[15];
		arm9->R[13] = 0x00803EC0;

        /* Initialize Memory */
        arm9->memory = init_memory_arm9();

        /* Initialize the data */
		arm9->R08_usr = 0; arm9->R08_fiq = 0;
	   	arm9->R09_usr = 0; arm9->R09_fiq = 0;
   		arm9->R10_usr = 0; arm9->R10_fiq = 0;
 		arm9->R11_usr = 0; arm9->R11_fiq = 0;
		arm9->R12_usr = 0; arm9->R12_fiq = 0;
		arm9->R13_usr = 0; arm9->R13_fiq = 0;
		arm9->R14_usr = 0; arm9->R14_fiq = 0;
		/* Init status data */
		arm9->cpsr.value = 0x00;
        arm9->cpsr.MODE = SYSTEM_MODE;
        arm9->newcpsr.value = arm9->cpsr.value;

        arm9->op.value = UNDEFINED;
        arm9->result.dword = 0x0;
        arm9->status = &arm9->cpsr;
		arm9->memory = init_memory_arm9();

		return arm9;
    }
    return NULL;

}

/* Executor */
void arm9_execute(ndsCPU *cpu)
{
    void (*decoder)(ndsCPU*);
    void (*executor)(ndsCPU*);

    /* Clear (important) result values */
    cpu->instruction = UNDEFINED;
    cpu->adressing = UNDEFINED;
	cpu->result.carry = 0;
    cpu->s_type = SET_NONE;
	
	PC = cpu->newpc;

    if( cpu->status->T ){ /* Thumb Mode */
        return; /* TODO, T mode not implemented */
    } else {
        decoder = &arm9_decoder;
        executor = &arm9_executor;

        cpu->op.value = read_word(PC, cpu->memory);

        if( !condition(cpu) )
            return;
    }

    cpu->newcpsr.value = cpu->status->value;
    cpu->set_flags = FALSE;

    decoder(cpu);
    executor(cpu);

	cpu->newpc += 4;

    /* If flags are changed, apply the changes */
    if( cpu->set_flags ){
        switch( cpu->s_type ){
        case SET_N_Z_C_V:
            cpu->status->V = ( cpu->result.most ) ? TRUE : FALSE;
        case SET_N_Z_C:
            cpu->status->C = ( cpu->result.most ) ? TRUE : FALSE;
        case SET_N_Z:
            cpu->status->N = BITSET(cpu->result.least, 31);
            cpu->status->Z = !cpu->result.least;
            break;
        case  SET_N_Z_OC_V:
            cpu->status->N = BITSET(cpu->result.least, 31);
            cpu->status->Z = !cpu->result.least;
            cpu->status->C = cpu->carry;
            cpu->status->V = (cpu->result.most) ? TRUE : FALSE;
            break;
        case SET_N_Z_RC_V:
            cpu->status->N = BITSET(cpu->result.least, 31) ? TRUE : FALSE;
            cpu->status->Z = !cpu->result.least;
            cpu->status->C = (cpu->result.most) ? FALSE : TRUE;
            cpu->status->V = (cpu->result.most) ? TRUE : FALSE;
            break;
        case SET_N_Z_RC_RV:
            cpu->status->N = BITSET(cpu->result.least, 31);
            cpu->status->Z = !cpu->result.least;
            cpu->status->C = (cpu->result.most) ? FALSE : TRUE;
            cpu->status->V = (cpu->result.most) ? FALSE : TRUE;
            break;
        case SET64_N_Z:
            cpu->status->N = BITSET(cpu->result.least, 31);
            cpu->status->Z = !cpu->result.dword;
            break;
        }
    }
}

u8 condition(ndsCPU *cpu)
{
    if( cpu->op.cond.val < GE ){
        if( (cpu->status->value & condition_mask[cpu->op.cond.val]) == condition_result[cpu->op.cond.val] ){
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        switch( cpu->op.cond.val ){
        case GE:
            return (cpu->status->N & cpu->status->V);
        case LT:
            return (cpu->status->N ^ cpu->status->V);
        case GT:
            return ((!cpu->status->Z) & (cpu->status->N & cpu->status->V) );
        case LE:
            return ( cpu->status->Z & (cpu->status->N ^ cpu->status->V) );
        case AL:
            return TRUE;
        }
    }
    return FALSE;
}
