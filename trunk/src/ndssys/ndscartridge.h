#ifndef NDSCART_H
#define NDSCART_H

#include "../types.h"

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

#endif
