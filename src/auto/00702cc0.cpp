#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterRenderScaled, 0x00627470, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwRasterUnlock, 0x00627490, (int a1));
CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterLock, 0x00627810, (int a1, int a2, int a3));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_00861a00_GetLockErrorCode, 0x00702BE0, (C_00861a00* self, void*));

using BinkFrameFunction = int (__stdcall*)(uint32_t);
using BinkCopyFunction = int (__stdcall*)(
    uint32_t, int, int, int, int, int, int);

CSF_GLOBAL(BinkFrameFunction, g_BinkNextFrame, 0x0082D398);
CSF_GLOBAL(BinkCopyFunction, g_BinkCopyToBuffer, 0x0082D39C);
CSF_GLOBAL(BinkFrameFunction, g_BinkDoFrame, 0x0082D3A0);
CSF_GLOBAL(BinkFrameFunction, g_BinkWait, 0x0082D3A4);

static int C_00861a00_RenderVideoRaster(C_00861a00* self)
{
    alignas(8) uint8_t rectangleBytes[0x10];
    CSF_AT(int, rectangleBytes, 0x00) = 0;
    CSF_AT(int, rectangleBytes, 0x04) = 0;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    CSF_AT(int, rectangleBytes, 0x08) = CSF_AT(int, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    const uint32_t raster = self->field_0014;
    CSF_AT(int, rectangleBytes, 0x0C) = CSF_AT(int, dimensions, 0x04);
    return RwRasterRenderScaled(
        static_cast<int>(raster), reinterpret_cast<int>(rectangleBytes));
}

static char __fastcall C_00861a00_UpdateVideoFrame(
    C_00861a00* self, void* /*edx*/)
{
    const uint8_t finished = self->field_0010;
    if (finished)
        return static_cast<char>(finished);

    LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    RwRasterClear(0);

    if (g_BinkWait(self->field_000c))
        return static_cast<char>(C_00861a00_RenderVideoRaster(self));

    g_BinkDoFrame(self->field_000c);
    const int pixels = RwRasterLock(static_cast<int>(self->field_0014), 0, 1);
    if (pixels)
    {
        const uint32_t video = self->field_000c;
        const int format = C_00861a00_GetLockErrorCode(self, nullptr);
        const int pitch = CSF_AT(
            int, reinterpret_cast<void*>(self->field_0014), 0x18);
        const int height = CSF_AT(int, reinterpret_cast<void*>(video), 0x04);
        const int copyResult =
            g_BinkCopyToBuffer(video, pixels, pitch, height, 0, 0, format);
        RwRasterUnlock(static_cast<int>(self->field_0014));
        if (!copyResult)
            C_00861a00_RenderVideoRaster(self);
    }

    const uint32_t video = self->field_000c;
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(video), 0x0C) !=
            CSF_AT(uint32_t, reinterpret_cast<void*>(video), 0x08) ||
        (self->field_0004 & 2) != 0)
    {
        return static_cast<char>(g_BinkNextFrame(video));
    }

    self->field_0010 = 1;
    return static_cast<char>(video);
}

HOOK(0x00702CC0, C_00861a00_UpdateVideoFrame, "auto");
