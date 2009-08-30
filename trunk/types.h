#ifndef DEFINES_H
#define DEFINES_H

typedef unsigned long   dword;
typedef unsigned int    word;
typedef unsigned short  hword;
typedef unsigned char   byte;

typedef unsigned            u32;
typedef unsigned long long  u64;

#define PACKED_      __attribute__((__packed__))
#define CDECL_      __attribute__((cdecl))

#endif
