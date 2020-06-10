#pragma once

typedef signed char        i8;
typedef short              i16;
typedef int                i32;
typedef long long          i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef char* string;
typedef char* char_ptr;

#define i8_min         (-127i8 - 1)
#define i16_min        (-32767i16 - 1)
#define i32_min        (-2147483647i32 - 1)
#define i64_min        (-9223372036854775807i64 - 1)

#define i8_max         127i8
#define i16_max        32767i16
#define i32_max        2147483647i32
#define i64_max        9223372036854775807i64

#define u8_max         0xffui8
#define u16_max        0xffffui16
#define u32_max        0xffffffffui32
#define u64_max        0xffffffffffffffffui64
