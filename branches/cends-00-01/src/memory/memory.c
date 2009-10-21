#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct ndsMEM *init_memory_arm9()
{
    ndsMEM *mem = malloc( sizeof(*mem) );
	
	mem->_main = malloc( sizeof(u8) *  0x501C800 );
	if( !mem->_main ){
		return NULL;
	}

	mem->ITCM = &mem->_main[0x0];
	mem->DTCM = &mem->_main[0x8000];
	mem->MAIN = &mem->_main[0xC000];
	mem->WRAM = &mem->_main[0x100C000];
	mem->REGS = &mem->_main[0x200C000];
	mem->STACK = &mem->_main[0x300C000];
	mem->PALT = &mem->_main[0x400C000];
	mem->VIDEO = &mem->_main[0x400C800];
	mem->SPRT = &mem->_main[0x500C800];
	mem->BIOS = &mem->_main[0x5014800];

	u16 temp = 0xFFDF;
	memcpy(&mem->MAIN[(0x027FFFFE - MAIN_MIN)%MAIN_SIZE], &temp, sizeof(u16));
	temp = 0xE732;
	memcpy(&mem->MAIN[(0x027FFFFE - MAIN_MIN)%MAIN_SIZE], &temp, sizeof(u16));	
	return mem;
}

void destroy_memory_arm9(struct ndsMEM *mem)
{
	assert(mem);
	assert(mem->_main);
	free(mem->_main);
	free(mem);
}

u32  read_word(u32 adr, ndsMEM *mem){
	assert(mem);
	u32 value = 0x00000000;
	switch(adr){
		case MAIN_MIN...MAIN_MAX:
			memcpy(&value, &mem->MAIN[adr%MAIN_SIZE], sizeof(u32));
	}
	return value;
}
u16  read_halfword(u32 adr, ndsMEM *mem){
    assert(mem);
	return 0xFFFF;
}
u8   read_byte(u32 adr, ndsMEM *mem){
    assert(mem);
	return 0xFF;
}
void write_word(u32 adr, u32 val, ndsMEM *mem){

}
void write_halfword(u32 adr, u16 val, ndsMEM *mem){
}
void write_byte(u32 adr, u8 val, ndsMEM *mem){

}
