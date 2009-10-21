#ifndef ENDSCPUH
#define ENDSCPUH

#include "arm9_defs.h"
#include "../memory/memory.h"
#include "../types.h"

typedef union arm9CPSR
{
    u32 value;
    struct {
        unsigned MODE : 5;
        unsigned T : 1;
        unsigned F : 1;
        unsigned I : 1;
        unsigned DNM : 1;
        unsigned Q : 1;
        unsigned V : 1;
        unsigned C : 1;
        unsigned Z : 1;
        unsigned N : 1;
    };
}arm9CPSR;

typedef struct ndsCPU
{
	void (*exec)(struct ndsCPU *cpu);
    /* Registers */
    union arm9CPSR *status; // Current cpsr pointer
    union arm9CPSR *saved_status; // Current spsr pointer
    union arm9CPSR cpsr; //Default cpsr
    union arm9CPSR newcpsr; //executor dummy cpsr
    u32 set_flags; // boolean
    u32 R08_usr;
    u32 R09_usr;
    u32 R10_usr;
    u32 R11_usr;
    u32 R12_usr;
    u32 R13_usr;
    u32 R14_usr;
    u32 R13_svc;
    u32 R14_svc;
    u32 SPSR_svc;
    u32 R13_abt;
    u32 R14_abt;
    u32 SPSR_abt;
    u32 R13_und;
    u32 R14_und;
    u32 R13_irq;
    u32 R14_irq;
    u32 SPSR_irq;
    u32 R08_fiq;
    u32 R09_fiq;
    u32 R10_fiq;
    u32 R11_fiq;
    u32 R12_fiq;
    u32 R13_fiq;
    u32 R14_fiq;
    u32 SPSR_fiq;
    /* Default Registers */
    u32 R[0x10];
	u32 *pc;
	u32 newpc; // swap pc register
    u32 rd, rn, rm, rs; // general swaps

    /* Opcode */
    union opcode_u op;

	struct {
        union {
            u64 dword;
            struct {
                u32 least;
                u32 most;
            };
        };
        u32 carry;
        u32 op1;
        u32 op2;
        u32 adress;
    } result;
    
    /* Decoded OP values */
    u32 operand, carry, adress, s_type;
    u32 instruction;
    u32 adressing;

    /* Memory */
    ndsMEM *memory;
}ndsCPU;

extern ndsCPU* create_cpu_arm9();
extern ndsCPU* create_cpu_arm7();

extern void destroy_cpu(ndsCPU *cpu);
extern void arm9_decoder(ndsCPU *cpu);
extern void arm9_executor(ndsCPU *cpu);

/* CONSTANTS, DEFINES */
#define USER_MODE	0x10
#define FIQ_MODE	0x11
#define IRQ_MODE	0x12
#define SUPER_MODE	0x13
#define ABORT_MODE	0x17
#define UNDEF_MODE	0x1B
#define SYSTEM_MODE	0x1F

#define EQ  0
#define NE  1
#define CS  0x02
#define CC  0x03
#define MI  0x04
#define PL  0x05
#define VS  0x06
#define VC  0x07
#define HI  0x08
#define LS  0x09
#define GE  0x0A
#define LT  0x0B
#define GT  0x0C
#define LE  0x0D
#define AL  0x0E
#define NV  0x0F

/* SET STATUS REGISTER TYPE */
#define SET_NONE        0x00
#define SET_N_Z         0x01
#define SET_N_Z_C       0x02
#define SET_N_Z_OC      0x03
#define SET_N_Z_OC_V    0x04
#define SET_N_Z_C_V     0x05
#define SET_N_Z_RC_V    0x06
#define SET_N_Z_C_RV    0x07
#define SET_N_Z_RC_RV   0x08
#define SET64_N_Z       0x09

#define SET_CARRY( a, b, c) ( (((a)&0x80000000)&((b)&0x80000000)) | ( (((a)&0x80000000)|((b)&0x80000000))&((~c)&0x80000000) ) )
#define SET_OVERFLOW( a, b, c)  ( ( ((a)&0x80000000) & ((b)&0x80000000) & ((~c)&0x80000000) ) | \
                                (   ((~a)&0x80000000) & ((~b)&0x80000000) & ((c)&0x80000000)) )

#endif //ENDSCPUH

