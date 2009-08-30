#ifndef NDSVIDEODEFS_H
#define NDSVIDEODEFS_H

#include "../types.h"

struct DISPCNT_S {
    unsigned bgmode : 2;
    unsigned bg02d_3d : 1;
    unsigned tile_obj_mapping : 1;
    unsigned bitmap_obj_2dim : 1;
    unsigned bitmap_obj_mapping : 1;
    unsigned forced_blank : 1;
    unsigned bg0 : 1;
    unsigned bg1 : 1;
    unsigned bg2 : 1;
    unsigned bg3 : 1;
    unsigned obj : 1;
    unsigned wnd0 : 1;
    unsigned wnd1 : 1;
    unsigned wndO : 1;
    unsigned dspmode : 2;
    unsigned vramblock : 2;
    unsigned tile_obj_boundary : 2;
    unsigned bitmap_obj_boundary : 1;
    unsigned obj_process_hblank : 1;
    unsigned char_base : 3;
    unsigned scrn_base : 3;
    unsigned bg_ex_pal : 1;
    unsigned obj_ex_pal : 1;
};
union DISPCNT_U {
    u32 val;
    struct DISPCNT_S bit;
};


#endif
