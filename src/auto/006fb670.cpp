#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(char, __fastcall, GfxScreenEffects_CreateRasters, 0x006FB5F0, (void* self, void*));
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0, (void* self, void* edx, int a2));

static char __fastcall C_00862d48_Initialize(
    C_00862d48* self, void* /*edx*/, int effectId)
{
    if (self->field_0008)
    {
        const uint32_t resource = self->field_0060;
        if (resource)
        {
            const int managerId = GetGlobalInt();
            const int manager = ResolveRwManager(managerId, nullptr);
            ReleaseManagedResource(
                reinterpret_cast<void*>(manager), nullptr,
                reinterpret_cast<void**>(resource));
        }
        self->field_0060 = 0;

        const uint32_t raster = CSF_AT(uint32_t, self, 0x64);
        if (raster)
        {
            RwRasterDestroy(raster);
            CSF_AT(uint32_t, self, 0x64) = 0;
        }
    }

    ScreenEffects_UpdateBase(self, nullptr);
    InitializeScreenEffect(self, nullptr, effectId);
    CSF_AT(uint32_t, self, 0x64) = 0;
    self->field_0060 = 0;

    void** effectVtable = CSF_AT(void**, self, 0);
    CSF_AT(uint8_t, self, 0x30) = 0;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        effectVtable[0x2C / 4])(self, nullptr);

    const char result = GfxScreenEffects_CreateRasters(self, nullptr);
    self->field_0008 = 1;
    if (!result)
    {
        const uint32_t resource = self->field_0060;
        if (resource)
        {
            const int managerId = GetGlobalInt();
            const int manager = ResolveRwManager(managerId, nullptr);
            ReleaseManagedResource(
                reinterpret_cast<void*>(manager), nullptr,
                reinterpret_cast<void**>(resource));
        }
        self->field_0060 = 0;

        const uint32_t raster = CSF_AT(uint32_t, self, 0x64);
        if (raster)
        {
            RwRasterDestroy(raster);
            CSF_AT(uint32_t, self, 0x64) = 0;
        }
        ScreenEffects_UpdateBase(self, nullptr);
    }
    return result;
}

HOOK(0x006FB670, C_00862d48_Initialize, "auto");
