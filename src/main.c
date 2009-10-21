#include "cpu/cpu.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
	#include "gui/debugger.h"
#endif

struct nds_header_t{
    u8    GameTitle[12];
    u8    GameCode[4];
    u16   MakerCode;
    u8    UnitCode;
    u8    EncryptionSeedSelect;
    u8    Devicecapacity;
    u8    Reserved00[9];
    u8    RomVersion;
    u8    Autostart;
    u32    ARM9rom_offset;
    u32    ARM9entry_adress;
    u32    ARM9ram_adress;
    u32    ARM9size;
    u32    ARM7rom_offset;
    u32    ARM7ram_adress;
    u32    ARM7size;
    u32    FileNameTableOffset;
    u32    FileNameTableSize;
    u32    FileAllocationOffset;
    u32    FileAllocationSize;
    u32    FileARM9OverlayOffset;
    u32    FileARM9OverlaySize;
    u32    FileARM7OverlayOffset;
    u32    FileARM7OverlaySize;
    u32    PortSettingNormalCommands;
    u32    PortSettingKey1Commands;
    u32    IconTitleOffset;
    u16   SecureAreaChecksum;
    u16   SecureAreaLoadingTimeout;
    u32    ARM9AutoLoadListRamAdress;
    u32    ARM7AutoLoadlistRamAdress;
    u64   SecureAreaDisable;
    u32    TotalUsedRomSize;
    u32    RomHeaderSize;
    u8    Reserved01[0x38];
    u8    NintendoLogo[0x9c];
    u16   NintendoLogoChecksum;
    u16   HeaderChecksum;
    u32    DebugRomOffset;
    u32    DebugSize;
    u32    Reserved02;
    u8    Reserved03[0x90];
}__attribute__((__packed__))nds_header_t;

int main(int argc, char* argv[])
{
    u32 x = 0xFF + 4;
    ndsCPU *arm9 = create_cpu_arm9();
	
#ifdef _DEBUG

	gtk_init (&argc, &argv);

	//u8 m_main[0x1000000]
	struct nds_header_t header;
	FILE *rom = fopen("dslinux.nds", "rb");
	fseek(rom, 0, SEEK_SET);
	fread(&header, sizeof(struct nds_header_t), sizeof(u8), rom);
	fseek(rom, header.ARM9rom_offset, SEEK_SET);

	fread(arm9->memory->MAIN, header.ARM9size, sizeof(u8), rom);

	arm9->newpc = header.ARM9entry_adress;
	arm9->R[15] = header.ARM9entry_adress;

	fclose(rom);

	endsdbg *debugger = endsdbg_create(arm9);
	
	if( debugger ){
		debugger->run(debugger);	
		endsdbg_destroy(debugger);	
		return 0;
	} else {
		return -1;
	}
	
	return 0;

#endif

	arm9->exec(arm9);

    return 0;
}

