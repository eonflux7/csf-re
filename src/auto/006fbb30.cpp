#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseEffectResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(char, __fastcall, FinishEffectCleanup, 0x007090F0, (void* self, void* edx));

static char __fastcall C_00862d48_ReleaseResources(C_00862d48* self, void* /*edx*/)
{
    if (self->field_0008)
    {
        for (int index = 0; index < 5; ++index)
        {
            uint32_t& resource = index == 0
                ? self->field_0060
                : CSF_AT(uint32_t, self, 0x60 + index * 4);

            if (resource)
            {
                void** resourceObject = reinterpret_cast<void**>(resource);
                int managerId = GetGlobalInt();
                int manager = ResolveRwManager(managerId, nullptr);
                ReleaseEffectResource(reinterpret_cast<void*>(manager), nullptr, resourceObject);
            }
            resource = 0;
        }
    }

    return FinishEffectCleanup(self, nullptr);
}

HOOK(0x006FBB30, C_00862d48_ReleaseResources, "auto");
