#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwCameraCreate, 0x006290B0, ());
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(void*, __fastcall, Polygon_GrowEdgeArray, 0x006D9D80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_006d24a0_ConfigureCamera, 0x006DA080, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006d2390_ResolveParam, 0x006DAFB0, (void* self, void* edx, int a2, int arg4, int a3));
CSF_FUNC(int, __cdecl, RegisterExitCallback, 0x007AEE43, (void (__cdecl* callback)()));

CSF_GLOBAL(uint32_t, g_Parameter50Resolver, 0x008BB710);
CSF_GLOBAL(uint32_t, g_Parameter20Resolver, 0x008BB714);
CSF_GLOBAL(uint32_t, g_ResolverInitializationFlags, 0x008BB718);

static char __fastcall S_006d24a0_InitializeCamera(
    S_006d24a0* self, void* /*edx*/)
{
    const uint32_t edgeCapacity = self->field_0010;
    if (edgeCapacity < 0x3E8)
        Polygon_GrowEdgeArray(&self->field_000c, nullptr,
                              static_cast<int>(1000 - edgeCapacity));

    if (self->field_0008)
        RwRasterDestroy(self->field_0008);
    if (self->field_0004)
        RwRasterDestroy(self->field_0004);

    self->field_0004 = 0;
    self->field_0008 = 0;

    const int camera = RwCameraCreate();
    self->field_00c4 = camera;
    if (camera)
    {
        void* frame = RwFrameCreate();
        _rwObjectHasFrameSetFrame(static_cast<int>(self->field_00c4),
                                 reinterpret_cast<int>(frame));
    }

    if ((g_ResolverInitializationFlags & 1) == 0)
    {
        g_ResolverInitializationFlags |= 1u;
        g_Parameter20Resolver =
            reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00861220));
        RegisterExitCallback(
            reinterpret_cast<void (__cdecl*)()>(CSF_ADDR_FN(0x0082B050)));
    }

    if ((g_ResolverInitializationFlags & 2) == 0)
    {
        g_ResolverInitializationFlags |= 2u;
        g_Parameter50Resolver =
            reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0086122C));
        RegisterExitCallback(
            reinterpret_cast<void (__cdecl*)()>(CSF_ADDR_FN(0x0082B040)));
    }

    char result = S_006d24a0_ConfigureCamera(self, nullptr);
    if (self->field_00c8 != 3 && result)
    {
        result = S_006d2390_ResolveParam(
            &self->field_0044, nullptr, 20,
            reinterpret_cast<int>(&g_Parameter20Resolver), 0);
        if (result)
        {
            return S_006d2390_ResolveParam(
                reinterpret_cast<uint8_t*>(self) + 0x84, nullptr, 50,
                reinterpret_cast<int>(&g_Parameter50Resolver), 0);
        }
    }
    return result;
}

HOOK(0x006DB160, S_006d24a0_InitializeCamera, "auto");
