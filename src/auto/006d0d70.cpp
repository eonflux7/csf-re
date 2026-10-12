#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* /*edx*/, int index));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* /*edx*/, int key, int value));

// Stores a byte flag into entry 4 of scene region 0 (reached through the global value's +4 pointer).
static int __stdcall GfxLights_SetRegionEntry4(uint8_t flagValue)
{
    int globalValue = GetGlobalValue_6C5760();
    uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalValue));
    C_0085e7b8* sceneRoot = *reinterpret_cast<C_0085e7b8**>(globalBase + 4);
    C_008670f0* sceneRegion = reinterpret_cast<C_008670f0*>(
        C_0085e7b8_GetSceneRegionAt(sceneRoot, nullptr, 0));
    return C_008670f0_SetEntry(sceneRegion, nullptr, 4, flagValue);
}

HOOK(0x006D0D70, GfxLights_SetRegionEntry4, "auto");
