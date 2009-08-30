#ifndef OPCODE_H
#define OPCODE_H


/* All of these are just to help avoid about a thousand
   or so bit-masks, and about a 100 macros */

struct op_regs_t{
    unsigned m : 4;
    unsigned : 4;
    unsigned s : 4;
    unsigned d : 4;
    unsigned n : 4;
    unsigned : 1;
    unsigned : 4;
    unsigned : 3;
    unsigned : 4;
};
struct op_iflags_t{
    unsigned : 20;
    unsigned s : 1;
    unsigned w : 1;
    unsigned b : 1;
    unsigned u : 1;
    unsigned p : 1;
    unsigned i : 1;
    unsigned : 2;
    unsigned : 4;
};
struct op_icpinfo_t{
    unsigned crm : 4;
    unsigned : 1;
    unsigned op2 : 3;
    unsigned cp_num : 4;
    unsigned crd : 4;
    unsigned crn : 4;
    unsigned L : 1;
    unsigned op1 : 3;
    unsigned : 4;
    unsigned : 4;
};
/* Some misc values */
struct op_iimd8bit_t {
    unsigned val : 8;
    unsigned : 24;
};
struct op_iimd12bit_t{
    unsigned val : 12;
    unsigned : 20;
};
struct op_iimd16bit_t{
    unsigned val : 16;
    unsigned : 16;
};
struct op_iimd24bit_t{
    unsigned val : 24;
    unsigned : 8;
};
struct op_irotate_t {
    unsigned : 8;
    unsigned val : 4;
    unsigned : 20;
};
struct op_ishift_t{
    unsigned : 5;
    unsigned oper : 2;
    unsigned amount : 5;
    unsigned : 20;
};
struct op_icond_t{
    unsigned : 28;
    unsigned val : 4;
};

/* total space used? 32 bits or 32 bits * 11 ? */
union opcode_u{
    u32 val;
    struct op_regs_t        R; /* registers */
    struct op_iflags_t      F; /* flags */
    struct op_icpinfo_t     CP;/* coprocessor */
    struct op_icond_t       cond;
    struct op_iimd8bit_t    b8;
    struct op_iimd12bit_t   b12;
    struct op_iimd16bit_t   b16;
    struct op_iimd24bit_t   b24;
    struct op_irotate_t     rotate;
    struct op_ishift_t      shift;

};


/* Thumb Instruction Set v5 */
struct THUMB_SHIFTi_IDENTIFIER{
    unsigned : 11;
    unsigned opcode : 2;
    unsigned ident : 3;
};
struct THUMB_ADDSUBTRACTir_IDENTIFIER{
    unsigned : 9;
    unsigned opc : 1;
    unsigned ident : 6; /* 000111b or 000110b */
};
struct THUMB_ADDSUBTRACT_COMPARE_MOVEi_IDENTIFIER{
    unsigned : 11;
    unsigned opcode : 2;
    unsigned ident : 3; /* 001b */
};
struct THUMB_DATAPROCESSINGr_IDENTIFIER{
    unsigned : 6;
    unsigned opcode : 4;
    unsigned ident : 6; /* 010000b */
};
struct THUMB_SPECIALDATAPROCESSING_IDENTIFIER{
    unsigned : 8;
    unsigned opcode : 2;
    unsigned ident : 6;
};
struct THUMB_BRANCHEXCHANGE_IDENTIFIER{
    unsigned : 8;
    unsigned ident : 8;
};
struct THUMB_LOADFROMLITERALPOOL_IDENTIFIER{
    unsigned : 11;
    unsigned ident : 7;
};
struct THUMB_LOADSTOREroff_IDENTIFIER{
    unsigned : 9;
    unsigned opcode : 3;
    unsigned ident : 4;
};
struct THUMB_LOADSTOREWORDBYTEioff_IDENTIFIER{
    unsigned : 13;
    unsigned ident : 3;
};
struct THUMB_LDSTACK_LDHWORD_ADDPC_OR_ADDSP_IDENTIFIER{
    unsigned : 12;
    unsigned ident : 4;
};
struct THUMB_MISC_ADJUSTSTACKPOINTER_IDENTIFIER{
    unsigned : 7;
    unsigned opcode : 1;
    unsigned ident : 8;
};
struct THUMB_MISC_PUSHPOPREGISTERLIST_IDENTIFIER{
    unsigned : 9;
    unsigned ident_misc : 2; /* 10b */
    unsigned : 1;
    unsigned ident_main : 5;
};
struct THUMB_MISC_SWI_IDENTIFIER{
    unsigned : 8;
    unsigned ident : 8;
};
struct THUMB_LOADSTOREMULTIPLE_IDENTIFIER{
    unsigned : 12;
    unsigned ident : 4;
};
struct THUMB_CONDITIONALBRANCH_IDENTIFIER{
    unsigned : 12;
    unsigned ident : 4;
};
struct THUMB_SWI_IDENTIFIER{
    unsigned : 8;
    unsigned ident : 8;
};
struct THUMB_UNCONDITIONALBRANCH_IDENTIFIER{
    unsigned : 11;
    unsigned ident : 5;
};
struct THUMB_BLX_SUFFIX_PREFIX_AND_BLSUFFIX_IDENTIFIER{
    unsigned : 11;
    unsigned ident : 5;
};

/* IDENTIFIER CONSTANTS FOR THUMB INSTRUCTION SET */
enum{
    /* In order of apearance */
    TM_SHIFTi_ID = 0,
    TM_ADDSUBTRACTr_ID = 6,
    TM_ADDSUBTRACTi_ID = 7,
    TM_ADDSUBTRACT_COMPARE_MOVEi_ID = 1,
    TM_DATAPROCESSING_R = 0x20,
    TM_SPECIALDATAPROCESSING_ID = 0x11,
    TM_BRANCHEXCHANGE_ID = 0x47,
    TM_LOADFROMLITERALPOOL_ID = 0x9,
    TM_LOADSTOREroff_ID = 0x5,
    TM_LOADSTOREWORDBYTEioff_ID = 0x3,
    TM_LOADSTOREHWORDioff_ID = 0x8,
    TM_LOADSTORESTACK_ID = 0x9,
    TM_ADDTOSPORPC_ID = 0xA,
    TM_MISC_ADJUSTSTACKPOINTER_ID = 0xB0,
    TM_MISC_PUSHPOPREGISTERLISTmisc_ID = 0x2,
    TM_MISC_PUSHPOPREGISTERLISTmain_ID = 0xB,
    TM_MISC_SWI_ID = 0xBE,
    TM_LOADSTOREMULTIPLE_ID = 0xC,
    TM_CONDITIONALBRANCH_ID = 0xD,
    TM_SWI_ID = 0xDF,
    TM_BLXSUFFIX_ID = 0x1D,
    TM_BLXPREFIX_ID = 0x1E,
    TM_BLSUFFIX_ID  = 0x1F
};

/* THUMB DECODER UNION */
union THUMB_OPCODE_BREAKDOWN{
	u32 val;
	struct THUMB_SHIFTi_IDENTIFIER a;
	struct THUMB_ADDSUBTRACTir_IDENTIFIER b;
	struct THUMB_ADDSUBTRACTir_IDENTIFIER c;
	struct THUMB_ADDSUBTRACT_COMPARE_MOVEi_IDENTIFIER d;
	struct THUMB_DATAPROCESSINGr_IDENTIFIER e;
	struct THUMB_SPECIALDATAPROCESSING_IDENTIFIER f;
	struct THUMB_BRANCHEXCHANGE_IDENTIFIER g;
	struct THUMB_LOADFROMLITERALPOOL_IDENTIFIER h;
	struct THUMB_LOADSTOREroff_IDENTIFIER i;
	struct THUMB_LOADSTOREWORDBYTEioff_IDENTIFIER j;
	struct THUMB_LDSTACK_LDHWORD_ADDPC_OR_ADDSP_IDENTIFIER k;
	struct THUMB_LDSTACK_LDHWORD_ADDPC_OR_ADDSP_IDENTIFIER l;
	struct THUMB_LDSTACK_LDHWORD_ADDPC_OR_ADDSP_IDENTIFIER m;
	struct THUMB_LDSTACK_LDHWORD_ADDPC_OR_ADDSP_IDENTIFIER n;
	struct THUMB_MISC_ADJUSTSTACKPOINTER_IDENTIFIER o;
	struct THUMB_MISC_PUSHPOPREGISTERLIST_IDENTIFIER p;
	struct THUMB_MISC_SWI_IDENTIFIER q;
	struct THUMB_LOADSTOREMULTIPLE_IDENTIFIER r;
	struct THUMB_CONDITIONALBRANCH_IDENTIFIER s;
	struct THUMB_SWI_IDENTIFIER t;
	struct THUMB_BLX_SUFFIX_PREFIX_AND_BLSUFFIX_IDENTIFIER u;
	struct THUMB_BLX_SUFFIX_PREFIX_AND_BLSUFFIX_IDENTIFIER v;
	struct THUMB_BLX_SUFFIX_PREFIX_AND_BLSUFFIX_IDENTIFIER w;
};

/* THUMB MODE OPCODE VALUES */
struct THUMB_OPC_REGISTERS{
	unsigned dn : 3;
	unsigned nm : 3;
	unsigned m  : 3;
	unsigned nd : 3;
	unsigned : 4;
};
struct THUMB_OPC_IMMEDIATE8bit{
	unsigned a : 8;
	unsigned b : 8;
};
struct THUMB_OPC_IMMEDIATE6bit{
	unsigned a : 6;
	unsigned b : 6;
	unsigned c : 6;
};
struct THUMB_OPC_IMMEDIATE4bit{
	unsigned a : 4;
	unsigned b : 4;
	unsigned c : 4;
	unsigned d : 4;
};
struct THUMB_OPC_OFFSET11bit{
	unsigned val : 11;
	unsigned : 5;
};
struct THUMB_OPC_BITS{
	unsigned : 6;
	unsigned h2 : 1;
	unsigned h1 : 1;
	unsigned lx : 1;
	unsigned : 2;
	unsigned l_sp : 1;
	unsigned b : 1;
	unsigned : 3;
};

/* THUMB MODE OPCODE */
union THUMB_OPCODE_VALUES{
	u16 val;
	struct THUMB_OPC_BITS 			F;
	struct THUMB_OPC_REGISTERS 		R;
	struct THUMB_OPC_IMMEDIATE8bit 	b8;
	struct THUMB_OPC_IMMEDIATE6bit	b6;
	struct THUMB_OPC_IMMEDIATE4bit  b4;
	struct THUMB_OPC_OFFSET11bit	offset;
};

#endif
