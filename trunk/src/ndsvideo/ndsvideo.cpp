#include "ndsvideo.h"
#include <cstring>

#ifndef NDS_DEBUG
void NDSVIDEO::render2D()
{
    dispcnt.val = *mem->m_regs;
    if( !dispcnt.bit.dspmode ){

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
        return;
    }
    switch(dispcnt.bit.bgmode)
    {
        case 0:
        {
            SDL_Rect pix;
            pix.w = 1;
            pix.h = 1;
            u16 col;
            for(pix.y = 0; pix.y < 192; pix.y ++){
                for(pix.x = 0; pix.x < 256; pix.x++){
                    memcpy(&col, &mem->m_video[(pix.y * 192)+(pix.x * 3)], sizeof(u16));
                    SDL_FillRect(screen, &pix, (col >> 1));
                }
            }
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
    }
}

#endif





