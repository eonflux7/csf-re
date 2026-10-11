#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __fastcall, RefreshParticleResources, 0x0051DFA0, (void* self, void* edx));

CSF_GLOBAL(const char*, g_psz_PFX_WATER_SPLASH, 0x008A8C88);

static uint32_t* __fastcall C_00840f28_LoadWaterSplash(
    C_00840f28* self, void* /*edx*/)
{
    void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    self->field_0010 = 0;

    int resourceIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, manager, 0)[3])(
                manager, nullptr, g_psz_PFX_WATER_SPLASH);

    if (resourceIndex != -1)
    {
        if (self->field_0010 >= self->field_000c)
        {
            (void)CSF_VTBL(0x0041E5F0);
            GrowButtonArray(
                reinterpret_cast<uint8_t*>(self) + 8,
                nullptr,
                CSF_AT(int, self, 0x14));
        }

        uint32_t index = self->field_0010;
        int* destination = reinterpret_cast<int*>(self->field_0008) + index;
        self->field_0010 = index + 1;
        *destination = resourceIndex;
    }

    return RefreshParticleResources(self, nullptr);
}

HOOK(0x0051E010, C_00840f28_LoadWaterSplash, "auto");
