#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t    u64;
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;
typedef int64_t     s64;
typedef int32_t     s32;
typedef int16_t     s16;
typedef int8_t      s8;

#define BIT(bval)   (1 << ((bval)))

#define PACKED_      __attribute__((__packed__))
#define CDECL_      __attribute__((cdecl))

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* CPU */
#define DISSABLEINT_G   0x10
#define ENABLEINT_G     0x11

#endif
