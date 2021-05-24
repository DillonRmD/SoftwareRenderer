#if !defined(RENDER_H)

#define DTB_MATH_USE_TYPES
#include "dtb_math.h"

#define internal static
#define global static

typedef unsigned long u64;   //8 byte(s)
typedef unsigned int u32;    // 4 byte(s)
typedef unsigned short u16;  // 2 byte(s)
typedef unsigned char u8;    // 1 byte(s)

typedef signed long s64;  // 8 byte(s)
typedef signed int s32;   // 4 byte(s)
typedef signed short s16; // 2 byte(s)
typedef signed char s8;   // 1 byte(s)

typedef double r64;
typedef float r32;

struct frame_buffer
{
    s32 Width, Height;
    u32 BytesPerPixel;
    u32 Pitch;
    void* Memory;
};

void Render(frame_buffer* Buffer);

#define RENDER_H
#endif