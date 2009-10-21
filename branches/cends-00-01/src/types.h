#ifndef TYPESH
#define TYPESH

#include <stdint.h>
#include <stddef.h>

#ifndef TRUE
	#define TRUE 1U
#endif
#ifndef FALSE
	#define FALSE 0U
#endif

typedef uint_least32_t	ebool;

typedef uint_least8_t   u8;
typedef uint_least16_t u16;
typedef uint_least32_t u32;
typedef uint_least64_t u64;

typedef int_least8_t s8;
typedef int_least16_t s16;
typedef int_least32_t s32;
typedef int_least64_t s64;

/* Some usefull defines */

#define BITSET(n, b) (((n)&(1<<(b))) ? TRUE : FALSE)

#define B0(n) ( (n) & 0x1 )
#define B1(n) ( (n) & 0x2 )
#define B2(n) ( (n) & 0x4 )
#define B3(n) ( (n) & 0x8 )
#define B4(n) ( (n) & 0x10 )
#define B5(n) ( (n) & 0x20 )
#define B6(n) ( (n) & 0x40 )
#define B7(n) ( (n) & 0x80 )
#define B8(n) ( (n) & 0x100 )
#define B9(n) ( (n) & 0x200 )
#define B10(n) ( (n) & 0x400 )
#define B11(n) ( (n) & 0x800 )
#define B12(n) ( (n) & 0x1000 )
#define B13(n) ( (n) & 0x2000 )
#define B14(n) ( (n) & 0x4000 )
#define B15(n) ( (n) & 0x8000 )
#define B16(n) ( (n) & 0x10000 )
#define B17(n) ( (n) & 0x20000 )
#define B18(n) ( (n) & 0x40000 )
#define B19(n) ( (n) & 0x80000 )
#define B20(n) ( (n) & 0x100000 )
#define B21(n) ( (n) & 0x200000 )
#define B22(n) ( (n) & 0x400000 )
#define B23(n) ( (n) & 0x800000 )
#define B24(n) ( (n) & 0x1000000 )
#define B25(n) ( (n) & 0x2000000 )
#define B26(n) ( (n) & 0x4000000 )
#define B27(n) ( (n) & 0x8000000 )
#define B28(n) ( (n) & 0x10000000 )
#define B29(n) ( (n) & 0x20000000 )
#define B30(n) ( (n) & 0x40000000 )
#define B31(n) ( (n) & 0x80000000 )

#endif //TYPESH
