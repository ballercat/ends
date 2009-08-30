#ifndef NDSSYS_H
#define NDSSYS_H

#include "../ndscpu/ndscpu.h"
#include "../ndsmem/ndsmem.h"

#ifndef NDS_DEBUG
    #include "../ndsvideo/ndsvideo.h"
    #include <SDL/SDL.h>
#endif

#include <cstdio>

class NDSSYSTEM
{

    public:

        NDSSYSTEM();
        virtual ~NDSSYSTEM();

        bool nds_loadfile(const char *fpath);

        int  parseinput();
        int  exec();

#ifndef NDS_DEBUG
    protected:

        SDL_Surface *screen;
        NDSVIDEO *vid;

        NDSMMU *mmu;
        NDSCPU *cpu;
        FILE *rom;
#else

        NDSMMU *mmu;
        NDSCPU *cpu;

    protected:

        FILE *rom;
#endif

};

#endif
