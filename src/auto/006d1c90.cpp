#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (signatures copied from the packet).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(char, __fastcall, LightWorld_ProcessLightEntry, 0x006D19B0, (void* self, void* edx, void* arg0, void* a3));

// Resolves the active clump manager from the global id and forwards the
// light record pointer and the extra argument to the world entry routine.
static char __cdecl GfxLights_ForwardLightEntryToActiveWorld(C_0085ec18* a3, C_00860288* arg0)
{
    int managerId = GetGlobalInt();
    void* clumpManager = reinterpret_cast<void*>(ResolveClumpManager(managerId, nullptr));
    return LightWorld_ProcessLightEntry(clumpManager, nullptr, arg0, a3);
}

HOOK(0x006D1C90, GfxLights_ForwardLightEntryToActiveWorld, "auto");
