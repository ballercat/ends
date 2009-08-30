#include "ndssys.h"
#include "ndscartridge.h"

#include <cstdio>
#include <cstring>

NDSSYSTEM::NDSSYSTEM()
{

    mmu = new NDSMMU;
    cpu = new NDSCPU(mmu);
#ifndef NDS_DEBUG
    if(!SDL_Init(SDL_INIT_EVERYTHING))
    {
        screen = SDL_SetVideoMode(256, 192*2, 16, SDL_DOUBLEBUF);
        if( screen )
            SDL_WM_SetCaption("e-NDS","e-NDS");
    }

    vid = new NDSVIDEO(screen,mmu);
#endif
    rom = NULL;
}

NDSSYSTEM::~NDSSYSTEM()
{
    delete(mmu);
    delete(cpu);
#ifndef NDS_DEBUG
    SDL_FreeSurface(screen);
#endif
    fclose(rom);
}

bool NDSSYSTEM::nds_loadfile(const char *fpath)
{
    struct nds_header_t header;
    rom = fopen(fpath, "rb");
    if( !rom )
        return false;
    else{
        fseek(rom, 0, SEEK_SET);
        fread(&header, sizeof(struct nds_header_t), sizeof(u8), rom);
        fseek(rom, header.ARM9rom_offset, SEEK_SET);

        /* Arm9 program */
        fread(mmu->memory, header.ARM9size, sizeof(u8), rom);
        memcpy(&mmu->memory[0x400000], mmu->memory, sizeof(u8)*0x400000);
        memcpy(&mmu->memory[0x800000], mmu->memory, sizeof(u8)*0x400000);
        memcpy(&mmu->memory[0xC00000], mmu->memory, sizeof(u8)*0x400000);

        /* Header */
        memcpy(&mmu->memory[0x7FFFE0], &header, sizeof(struct nds_header_t) );

        cpu->m_newpc = header.ARM9entry_adress;
        cpu->m_r[15] = header.ARM9entry_adress;

        cpu->arm9_entry = cpu->m_newpc;
        cpu->arm7_entry = header.ARM9entry_adress;

        cpu->arm9size = header.ARM9size;
        cpu->arm7size = header.ARM7size;

        return true;
    }
    return false;
}

#ifndef NDS_DEBUG
int NDSSYSTEM::exec()
{
    while( parseinput() != -1 )
    {
        cpu->exec();
        vid->render2D();
        SDL_Flip(screen);

    }
    return 0;
}

int NDSSYSTEM::parseinput()
{
    SDL_Event event;
    while( SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                return 0;
            case SDL_KEYUP:
                return 1;
        }
    }

    return 0;
}
#endif
