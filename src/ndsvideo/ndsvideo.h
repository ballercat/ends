#ifndef NDSVIDEO_H
#define NDSVIDEO_H

#include "../types.h"
#include "ndsvideodefs.h"
#include "../ndsmem/ndsmem.h"
#include <SDL/SDL.h>

class NDSVIDEO
{
    public:

        NDSVIDEO(SDL_Surface *pvid,NDSMMU *pmem) : screen(pvid),mem(pmem) {};
        virtual ~NDSVIDEO(){};

        void render2D(void);

    protected:


        SDL_Surface *screen;
        NDSMMU      *mem;
        union DISPCNT_U dispcnt;

};

#endif
