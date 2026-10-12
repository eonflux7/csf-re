#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound callees (signatures copied from the packet).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(char, __fastcall, LightWorld_UpdateLightEntry, 0x006D17B0, (void* self, void* edx, unsigned int* arg0, int a3));

// Resolves the active clump manager from the global id and forwards the
// light record pointer and the flag/argument to the world update routine.
static char __cdecl GfxLights_ForwardToActiveWorld(int lightArg, unsigned int* recordPtr)
{
    int managerId = GetGlobalInt();
    void* clumpManager = reinterpret_cast<void*>(ResolveClumpManager(managerId, nullptr));
    return LightWorld_UpdateLightEntry(clumpManager, nullptr, recordPtr, lightArg);
}

HOOK(0x006D1C70, GfxLights_ForwardToActiveWorld, "auto");
