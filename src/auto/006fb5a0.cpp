#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(char, __fastcall, ReleaseBaseScreenEffect, 0x007090F0, (void* self, void* edx));

static char __fastcall C_00862cd4_ReleaseScreenEffectResources(
    C_00862cd4* self, void* /*edx*/)
{
    if (self->field_0008)
    {
        if (self->field_0060)
        {
            void** resource = reinterpret_cast<void**>(self->field_0060);
            int globalManager = GetGlobalInt();
            int manager = ResolveRwManager(globalManager, nullptr);
            ReleaseManagedResource(reinterpret_cast<void*>(manager), nullptr, resource);
        }

        self->field_0060 = 0;

        if (self->field_0064)
        {
            RwRasterDestroy(self->field_0064);
            self->field_0064 = 0;
        }
    }

    return ReleaseBaseScreenEffect(self, nullptr);
}

HOOK(0x006FB5A0, C_00862cd4_ReleaseScreenEffectResources, "auto");
