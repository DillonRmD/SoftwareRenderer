#include "render.h"

internal void RenderRectangle(frame_buffer* Buffer, s32 MinX, s32 MinY, s32 MaxX, s32 MaxY, u32 Color)
{   
    u8* Row = (u8*)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch;
    
    for(s32 y = MinY; y < MaxY; y++)
    {
        u32* Pixel = (u32*)Row;
        for(s32 x = MinX; x < MaxX; x++)
        {
            *Pixel++ = Color;
        }
        Row += Buffer->Pitch;
    }
}

internal void ClearScreenColor(frame_buffer* Buffer, u32 Color)
{
    u32* Pixel = (u32*)Buffer->Memory;
    for(s32 Y = 0; Y < Buffer->Height; ++Y)
    {
        for(s32 X = 0; X < Buffer->Width; ++X)
        {
            *Pixel++ = Color;
        }
    }
}

internal void StressTest(frame_buffer* Buffer)
{
    s32 X = 0;
    s32 Y = 0;

    for (s32 J = 0; J < 275; ++J)
    {
        s32 Width = X + 50;
        s32 Height = Y + 50;

        if (J % 2 == 0)
        {
            RenderRectangle(Buffer, X, Y, Width, Height, 0xFF00FF00);
        }
        else
        {
            RenderRectangle(Buffer, X, Y, Width, Height, 0xFFFF0000);
        }

        X += 25;
        if (J % 50 == 0)
        {
            X = 0;
            Y += 100;
        }
    }
}

internal void RenderHorizontalLine(frame_buffer* Buffer, v2 p0, v2 p1, u32 Color)
{
    s32 MinX = p0.x;
    s32 MinY = p0.y;

    s32 MaxX = p1.x;
    s32 MaxY = p1.y;

    u8* Row = (u8*)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch;
    for(s32 X = MinX; X < MaxX; ++X)
    {
        u32* Pixel = (u32*)Row;
        *Pixel++ = Color;
        Row += Buffer->BytesPerPixel; // NOTE(Dillon): We increment by the bytesperpixel since we are rendering horizontally
    }
}

internal void RenderVerticalLine(frame_buffer* Buffer, v2 p0, v2 p1, u32 Color)
{
    s32 MinX = p0.x;
    s32 MinY = p0.y;

    s32 MaxX = p1.x;
    s32 MaxY = p1.y;

    u8* Row = (u8*)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch;

    
    for(s32 Y = MinY; Y < MaxY; ++Y)
    {
        u32* Pixel = (u32*)Row;
        *Pixel++ = Color;
        Row += Buffer->Pitch; //NOTE(Dillon): We increment by the pitch of the frame buffer since we are rendering strictly verticle
    }
}

internal void RenderDiagonalLine(frame_buffer* Buffer, v2 p0, v2 p1, u32 Color)
{
    s32 MinX = p0.x;
    s32 MinY = p0.y;

    s32 MaxX = p1.x;
    s32 MaxY = p1.y;

    u8* Row = (u8*)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch;

    for(s32 Y = MinY; Y < MaxY; ++Y)
    {
        u32* Pixel = (u32*)Row;
        for(s32 X = MinX; X < MaxX; ++X)
        {   
            *Pixel++ = Color;
            Row += Buffer->BytesPerPixel;
        }
        Row += Buffer->Pitch;
    }
}

void Render(frame_buffer* Buffer)
{
    RenderDiagonalLine(Buffer, dtbVec2(50, 100), dtbVec2(100, 300), 0xFF0000FF);
    RenderVerticalLine(Buffer, dtbVec2(50, 100), dtbVec2(100, 300), 0xFF00FF00);
    RenderHorizontalLine(Buffer, dtbVec2(50, 100), dtbVec2(200, 100), 0xFFFF0000);
}