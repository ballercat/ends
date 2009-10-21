#ifndef ENDSOPCODESH
#define ENDSOPCODESH

/*------ Instruction List -----*/
/* Instructions that are non decodable */
#define    UNDEFINED   0
/* Instructons that are decodable but are not emulated */
#define    UNIMPLEMENTED   74

/* ****************************************************
   *          AMR 9 32-bit Instructions SET           *
   **************************************************** */
/* Data Processing Instructons */
#define AND     1
#define EOR     2
#define SUB     3
#define RSB     4
#define ADD     5
#define ADC     6
#define SBC     7
#define RSC     8
#define TST     9
#define TEQ     10
#define CMP     11
#define CMN     12
#define ORR     13
#define MOV     14
#define BIC     15
#define MVN     16
/* Multiply Instructions, Normal & Long */
/* Normal Multiply */
#define MUL     0x11
#define MLA     0x12
/* Long Multiply */
#define SMULL   0x13
#define UMULL   0x14
#define SMLAL   0x15
#define UMLAL   0x16
/* Branch Instructions, B, BL, BLX, BX */
#define BR      0x17
#define BL      0x18
#define BLX     0x19
#define BX      0x1A
/* Miscellaneous Arithmetic instructions, CLZ */
#define CLZ 0x1B
/* Status Register Acess instructions, MRS, MSR */
#define MRS     0x1C
#define MSR     0x1D
/* Load And Store Instructions, LDR, LDRB, LDRBT, LDRH, LDRSB, LDRSH, LDRT, STR, STRB, STRBT, STRH, STRT */
#define LDR     0x1E
#define LDRB    0x1F
#define LDRBT   0x20
#define LDRBH   0x21
#define LDRH    0x22
#define LDRSB   0x23
#define LDRSH   0x24
#define LDRT    0x25
#define STR     0x26
#define STRB    0x27
#define STRBT   0x28
#define STRH    0x29
#define STRT    0x2A
/* Load And Store Multiple Instructions, LDM_1, LDM_2(User Registers Load Multiple)
   LDM_3(Load Multiple with Restore CPSR), STM_1, STM_2(User Registers Store Multiple) */
#define LDM_1   0x2B
#define LDM_2   0x2C
#define LDM_3   0x2D
#define STM_1   0x2E
#define STM_2   0x2F
/* Semaphore Instructions */
#define SWP     0x30
#define SWPB    0x31
/* Exception-Generating Instructions */
#define BKPT    0x32
#define SWI     0x33
/* Coprocessor Instructions */
#define CDP     0x34
#define LDC     0x35
#define MCR     0x36
#define MRC     0x37
#define STC     0x38
/* Extended Instructons */
#define QADD    0x39
#define QSUB    0x3A
#define QDADD   0x3B
#define QDSUB   0x3C
#define SMLAXY  0x3D
#define SMLAWY  0x3E
#define SMULWY  0x3F
#define SMLALXY 0x40
#define SMULXY  0x41
#define LDRD    0x42
#define STRD    0x43
#define PLD     0x44
#define STC2    0x45
#define LDC2    0x46
#define CDP2    0x47
#define MCR2    0x48
#define MRC2    0x49

/* THUMB MODE INSTRUCTIONS */
#define TADC    0x4B
#define TADD    0x4C
#define TAND    0x4D
#define TASR    0x4E
#define TBR     0x4F
#define TBIC    0x50
#define TBKPT   0x51
#define TBL     0x52
#define TBLX    0x53
#define TBX     0x54
#define TCMN    0x55
#define TCMP    0x56
#define TEOR    0x57
#define TLDMIA  0x58
#define TLDR    0x59
#define TLDRB   0x5A
#define TLDRH   0x5B
#define TLDRSB  0x5C
#define TLDRSH  0x5D
#define TLSL    0x5E
#define TLSR    0x5F
#define TMOV    0x60
#define TMUL    0x61
#define TMVN    0x62
#define TNEG    0x63
#define TORR    0x64
#define TPOP    0x65
#define TPUSH   0x66
#define TROR    0x67
#define TSBC    0x68
#define TSTMIA  0x69
#define TSTR    0x6A
#define TSTRB   0x6B
#define TSTRH   0x6C
#define TSUB    0x6D
#define TSWI    0x6E
#define TTST    0x6F


#define _COutBit    0x80000000
#define SIGNBIT    _COutBit

/* Instruction Masks(im prefix)*/
#define mINSTRUCTION    0xFF
/* ADRESSING MODES */
#define AD_DATA_IMMEDIATE				1
#define AD_DATA_REGISTER				2
#define AD_DATA_LSL_IMMEDIATE			3
#define AD_DATA_LSR_IMMEDIATE			4
#define AD_DATA_ASR_IMMEDIATE			5	
#define AD_DATA_ROR_IMMEDIATE			6
#define AD_DATA_RRX_IMMEDIATE			7
#define AD_DATA_LSL_REGISTER			8
#define AD_DATA_LSR_REGISTER			9
#define AD_DATA_ASR_REGISTER			10
#define AD_DATA_ROR_REGISTER			11
#define AD_LOAD_OFFSET_IMMEDIATE		12
#define AD_LOAD_OFFSET_REGISTER			13
#define AD_LOAD_PRE_IMMEDIATE			14
#define AD_LOAD_PRE_REGISTER			15
#define AD_LOAD_POST_REGISTER			16
//#define AD_LOAD_OFFSET_REGISTER			16
#define AD_LOAD_SCALED_OFFSET			17
#define AD_LOAD_SCALED_PRE				18
#define AD_LOAD_SCALED_POST				19
#define AD_MISCLOAD_OFFSET_IMMEDIATE	20
#define AD_MISCLOAD_PRE_IMMEDIATE		21
#define AD_MISCLOAD_POST_IMMEDIATE		22
#define AD_MISCLOAD_OFFSET_REGISTER		23

/* Instruction Type defines */
#define _DATA           0x0080000
#define _LOADSTORE      0x0100000
#define _BRANCH         0x0200000
#define _COPROCESSOR    0x0400000
#define _MISC           0x0800000
#define _DSP            0x1000000

#define ADRESSING_MASK     0x1F80000

/* Adressing type defines */
#define Rt      0x100
#define It      0x200

/* Data Processing */
#define ISt     0x400
#define RSt     0x800
#define DATA_MASK   0xF00

/* Regular Load Store */
#define POSTt   ISt
#define PREt    RSt
#define SCALEDt 0x1000
#define OFFSETt 0x2000

#define IMMEDIATE_OFFSETt   (It | OFFSETt)
#define REGISTER_OFFSETt    (Rt | OFFSETt)
#define SCALED_OFFSET_Rt    (Rt | SCALEDt | OFFSETt)
#define IMMEDIATE_PREt      (It | PREt)
#define REGISTER_PREt       (Rt | PREt)
#define IMMEDIATE_POSTt     (It | POSTt)
#define REGISTER_POSTt      (Rt | POSTt)
#define SCALED_POST_Rt      (Rt | POSTt | SCALEDt)
#define SCALED_PRE_Rt       (Rt | PREt | SCALEDt)
#define LOADSTORE_MASK      0x3F00

/* **** Simple Instruction Mask macro's **** */
#define imRlist(val, reg)   ( (val) & (1 << (reg)) )
/* Immediate */
#define _I      0x2000000
/* Modify PSR */
#define _S      0x100000
/* Load/Store */
#define _L      0x100000
/* P, U, B, W, L */
#define _P      0x1000000
#define _U      0x800000
#define _B      0x400000
#define _W      0x200000
/* P, U, B, W, L, S, I masks */
#define imS(op)    ( (op) & 0x100000 )
#define imW(op)    ( (op) & 0x200000 )
#define imB(op)    ( (op) & 0x400000 )
#define imU(op)    ( (op) & 0x800000 )
#define imP(op)    ( (op) & 0x1000000)
#define imI(op)    ( (op) & 0x2000000)
/* General Masks */
#define imIPUBWLS(op)   ( ((op)>>0x14) & 0x3F )
#define imRn(op)    ( ((op)>>0x10) & 0xF )
#define imRd(op)    ( ((op)>>0x0C) & 0xF )
#define imRs(op)    ( ((op)>>0x08) & 0xF )
#define imRm(op)    ( (op) & 0xF )
#define imShiftAmount(op) ( ((op) >> 7) & 0x1F )
#define imshift(op) ( ((op) >> 5) & 2 )
#define imImd8b(op)  ( (op) & 0xFF )
#define imImd12b(op) ( (op) & 0xFFF )
#define imImd24b(op) ( (op) & 0xFFFFFF )

#define imOpcode1(op) ( ((op) >> 0x14) & 0xF )
#define imopcode1(op) ( (imOpcode1((op))) >> 1 )
#define imOpcode2(op) ( ((op) >>  5 ) & 7 )

#define imSWBImmed(op) ( ((op) >> 8 ) & 0xFFF )
/* Generic Nb = 4-bit niblet, num is one of 8 niblets */
#define imGetNib(op, num)    ( ((op) >> (4*(num))) & 0xF )

/* duplicates */
/* Rn */
#define imMask(op)  imRn((op))
#define imCRn(op)   imRn((op))
#define imRdHi(op)  imRn((op))
/* Rd */
#define imCRd(op)   imRd((op))
#define imRdLo(op)  imRd((op))
/* Rs */
#define imRotate(op)    imRs((op))
#define imCp_num(op)    imRs((op))
#define imHiOffset(op)  imRs((op))
/* Rm */
#define imCRm(op)   imRm((op))
#define imLoOffset(op)  imRm((op))
#define imimmed(op) imRm((op))

/* Combinations */
#define imRdHiLo(op)    ((imRdHi((op)) << 0x4)|(imRdLo((op))))
#define imHiLoOffset(op) ((imHiOffset((op)) << 0x4)|(imLoOffset((op))))

#endif

