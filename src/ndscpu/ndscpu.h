#ifndef NDSCPU_H
#define NDSCPU_H

#include "../types.h"
#include "../ndsmem/ndsmem.h"
#include "opcode.h"

#ifndef PC
    #define PC  15
#endif

#define SET_CARRY( a, b, c) ( (((a)&SIGNBIT)&((b)&SIGNBIT)) | ( (((a)&SIGNBIT)|((b)&SIGNBIT))&((~c)&SIGNBIT) ) ) ? 1 : 0
#define SET_OVERFLOW( a, b, c)  ( ( ((a)&SIGNBIT) & ((b)&SIGNBIT) & ((~c)&SIGNBIT) ) | \
                                (   ((~a)&SIGNBIT) & ((~b)&SIGNBIT) & ((c)&SIGNBIT)) ) ? 1 : 0

class NDSCPU
{
    public:
        NDSCPU(NDSMMU *ptr);
        virtual ~NDSCPU(){};

        /* PSR */
        struct cpsr_t{
            unsigned mode : 5;
            unsigned t : 1;
            unsigned f : 1;
            unsigned i : 1;
            unsigned dnm : 19;
            unsigned q : 1;
            unsigned v : 1;
            unsigned c : 1;
            unsigned z : 1;
            unsigned n : 1;
        };
        union cpsr_u{
            u32    val;
            struct cpsr_t bit;
        };

        u32 m_r[0x10];
        union opcode_u m_op;
        union THUMB_OPCODE_VALUES m_thumb_op;

        u32 m_instruction;
        u32 m_adressing;
        union cpsr_u cpsr;
        union cpsr_u spsr;
        union cpsr_u spsr_svc;
        union cpsr_u spsr_abr;
        union cpsr_u spsr_und;
        union cpsr_u spsr_irq;
        union cpsr_u spsr_fiq;
        u32 m_value;
        u32 m_adress;
        u32 m_newpc;

        bool canexec;
        bool enabled;
        bool thumb_mode;

        void reset();
        void setenable(bool flag) { enabled = flag; };
        bool isenabled(void) { return enabled; };
        bool canexecute(void) { return canexec; };
        void exec(void);

        bool DCONLY;

        u32 arm9size;
        u32 arm7size;

        u32 arm9_entry;
        u32 arm7_entry;

        u32 swi;

    protected:

        /* Arm Mode */
        void cpu_decoder(void);
        void cpu_executor(void);

        /* Thumb Mode */
        void thumb_decoder(void);
        void thumb_executor(void);

        union THUMB_OPCODE_BREAKDOWN thumbid;

		int m_write(u32& val, u32& adress, int sz);

        bool iscond(void);
        bool interrupt;

        enum {
            USER = 0x10,
            FIQ = 0x11,
            IRQ = 0x12,
            SUPER = 0x13,
            ABORT = 0x17,
            UNDEF = 0x1B,
            SYSTEM = 0x1F
        };


        void modeswitch(u32 mode);


        u32 mode_saves[23]; /* system | svc | abr | und | irq | fiq */
        u32 mode_spot[0xFF];
        u32 mode_size[0xFF];




        NDSMMU *mem;

        /* FIXME: this is ugly */
        void altdecode();
        void dcDataI();
        void dcDataR();
        void dcDataIS();
        void dcDataRS();
        void dcBranch();
        void dcImmediateOffset();
        void dcRegisterOffset();
        void dcScaledOffsetR();
        void dcImmediatePre();
        void dcRegisterPre();
        void dcScaledPreR();
        void dcImmediatePost();
        void dcRegisterPost();
        void dcScaledPostR();

        /* Instructions */
        /* NOTE : Most of these are unnecessary */
        void m_undefined(void);
        void m_unimplemented(void);
        void m_and(void);     void m_mul(void);     void m_ldr(void);     void m_swp(void);
        void m_eor(void);     void m_mla(void);     void m_ldrb(void);    void m_swpb(void);
        void m_sub(void);     void m_smull(void);    void m_ldrbt(void);   void m_bkpt(void);
        void m_rsb(void);     void m_umull(void);   void m_ldrbh(void);   void m_swi(void);
        void m_add(void);     void m_smlal(void);   void m_ldrh(void);    void m_cdp(void);
        void m_adc(void);     void m_umlal(void);   void m_ldrsb(void);   void m_ldc(void);
        void m_sbc(void);            void m_ldrsh(void);   void m_mcr(void);
        void m_rsc(void);           void m_ldrt(void);    void m_mrc(void);
        void m_tst(void);          void m_str(void);     void m_stc(void);
        void m_teq(void);           void m_strb(void);
        void m_cmp(void);     void m_clz(void);     void m_strh(void);
        void m_cmn(void);     void m_mrs(void);     void m_strt(void);
        void m_orr(void);     void m_msr(void);     void m_ldm1(void);
        void m_mov(void);                           void m_ldm2(void);
        void m_bic(void);                           void m_ldm3(void);
        void m_mvn(void);                           void m_stm1(void);
                                                    void m_strbt(void);
                                                    void m_stm2(void);
        void m_qadd(void);
        void m_qsub(void);
        void m_qdadd(void);
        void m_qdsub(void);
        void m_smlaxy(void);
        void m_smlawy(void);
        void m_smulwy(void);
        void m_smlalxy(void);
        void m_smulxy(void);
        void m_ldrd(void);
        void m_strd(void);
        void m_pld(void);
        void m_stc2(void);
        void m_ldc2(void);
        void m_cdp2(void);
        void m_mcr2(void);
        void m_mrc2(void);

        void m_branch(void);

};

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

#endif


