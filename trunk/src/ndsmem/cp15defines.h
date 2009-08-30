#ifndef CP15DEFS_H
#define CP15DEFS_H

#include "../types.h"

/* These are probably only usefull in some internal functions only */
struct CPREG000_S {
    unsigned implementor : 8;
    unsigned variant : 4;
    unsigned arch : 4;
    unsigned part : 12;
    unsigned version : 4;
};
union CPREG000_U {
    u32 val;
    struct CPREG000_S bit;
};
struct CPREG001_S {
    unsigned : 3;
    unsigned cache_type : 4;
    unsigned harvard : 1;
    unsigned : 2;
    unsigned dcache_size : 4;
    unsigned dcache_associativity : 3;
    unsigned dcache_basesize : 1;
    unsigned dcache_wordsperline : 2;
    unsigned : 2;
    unsigned icache_size : 4;
    unsigned icache_associativity : 3;
    unsigned icache_basesize : 1;
    unsigned icache_wordsperline : 2;
};
union CPREG001_U {
    u32 val;
    struct CPREG001_S bit;
};
struct CPREG002_S {
    unsigned : 10;
    unsigned data_ram_size : 4;
    unsigned : 3;
    unsigned data_ram_absent : 1;
    unsigned : 4;
    unsigned instruction_ram_size : 4;
    unsigned : 3;
    unsigned instructioN_ram_absent : 1;
    unsigned : 2;
};
union CPREG002_U {
    u32 val;
    struct CPREG002_S bit;
};
struct CPREG100_S {
/*    unsigned : 12;
    unsigned itcm_loadmode : 1;
    unsigned itcm_enable : 1;
    unsigned dtcm_loadmode : 1;
    unsigned dtcm_enable : 1;
    unsigned tbit : 1;
    unsigned round_robin : 1;
    unsigned vbit : 1;
    unsigned icache_enable : 1;
    unsigned : 4;
    unsigned bigendian : 1;
    unsigned : 4;
    unsigned dcache_enable : 1;
    unsigned : 1;
    unsigned enabled : 1;
};*/
    unsigned enabled : 1;
    unsigned : 1;
    unsigned dcache_enable : 1;
    unsigned : 4;
    unsigned bigendian : 1;
    unsigned : 4;
    unsigned icache_enable : 1;
    unsigned vbit : 1;
    unsigned round_robin : 1;
    unsigned tbit : 1;
    unsigned dtcm_enable : 1;
    unsigned dtcm_loadmode : 1;
    unsigned itcm_enable : 1;
    unsigned itcm_loadmode : 1;
    unsigned : 12;
};
union CPREG100_U {
    struct CPREG100_S bit;
    u32 val;
};
/* Acess Permission Register c5 */
struct CPREG500_S {
    unsigned a0 : 4;
    unsigned a1 : 4;
    unsigned a2 : 4;
    unsigned a3 : 4;
    unsigned a4 : 4;
    unsigned a5 : 4;
    unsigned a6 : 4;
    unsigned a7 : 4;
};
union CPREG500_U {
    u32 val;
    struct CPREG500_S bit;
};
/* Protection Region Register(s) c6 */
struct CPREG600_S {
    unsigned enabled : 1;
    unsigned size : 5;
    unsigned : 6;
    unsigned base : 20;
};
union CPREG600_U {
    u32 val;
    struct CPREG600_S bit;
};
/* TCM Region Register c9 */
struct CPREG900_S {
    unsigned enabled : 1;
    unsigned : 1;
    unsigned : 6;
    unsigned base_adress : 26;
};
union CPREG900_U {
    u32 val;
    struct CPREG900_S bit;
};

#endif
