#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: RpPrtStdPropTabGetPropOffset (cdecl, 2 args, pops 0)
CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));

static int __cdecl ParticleRender_ZeroPropEntry11(int a1, int a2)
{
    (void)a1;
    uint8_t* entryBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));

    // a2 -> +0x0C holds a pointer; that pointer's +0x10 is the property table.
    uint32_t innerObject = CSF_AT(uint32_t, entryBase, 0x0C);
    uint32_t propTable = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(innerObject)), 0x10);

    int propOffset = RpPrtStdPropTabGetPropOffset(static_cast<int>(propTable), 11);

    CSF_AT(uint32_t, entryBase, propOffset) = 0;
    return a2;
}

HOOK(0x007038E0, ParticleRender_ZeroPropEntry11, "auto");
