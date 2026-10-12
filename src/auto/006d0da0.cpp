#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* /*edx*/, int index));
CSF_FUNC(int*, __fastcall, C_0085e7b8_ResolveChainedValue, 0x006C48C0, (C_0085e7b8* self, void* /*edx*/, int* a2, int a3));

// Reads the first byte of the value that the scene region chain resolves to.
static char __fastcall C_00860e68_GetSceneRegionFlagByte(C_00860e68* /*self*/, void* /*edx*/)
{
    int globalValue = GetGlobalValue_6C5760();
    void* globalBase = reinterpret_cast<void*>(globalValue);
    C_0085e7b8* sceneOwner = CSF_AT(C_0085e7b8*, globalBase, 4);

    C_0085e7b8* sceneRegion = reinterpret_cast<C_0085e7b8*>(
        C_0085e7b8_GetSceneRegionAt(sceneOwner, nullptr, 0));

    int a2 = 0;
    int* resolved = C_0085e7b8_ResolveChainedValue(sceneRegion, nullptr, &a2, 4);
    return *reinterpret_cast<char*>(resolved);
}

HOOK(0x006D0DA0, C_00860e68_GetSceneRegionFlagByte, "auto");
