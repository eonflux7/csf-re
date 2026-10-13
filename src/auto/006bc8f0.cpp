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

// Callee bindings (by address).
CSF_FUNC(int, __cdecl, GfxObjectProps_ReadTableEntryField4, 0x006B8820, (int a1));
CSF_FUNC(int, __cdecl, RpWorldSectorForAllAtomics, 0x0058DE50, (int a1, int (__cdecl* a2)(int, int), int a3));

// Original's __thiscall vtable slot 0x70 (slot 28): bool Accept(this, uint32_t* props, int tableOffset, int flags).
typedef unsigned char (__fastcall* GfxAcceptVirtualFn)(void* self, void* edx, uint32_t* props, int tableOffset, int flags);

// Callback run for each atomic of the sector (original: sub_6BC220).
typedef int (__cdecl* GfxAtomicVisitorFn)(int, int);

static uint32_t* __cdecl GfxEnvironment_VisitAtomicsIfAccepted(uint32_t* self, int** request)
{
    // Copy the six props dwords at self[18..23] into a local block.
    uint32_t props[6];
    props[3] = self[21];
    props[4] = self[22];
    props[5] = self[23];
    props[0] = self[18];
    props[1] = self[19];
    props[2] = self[20];

    int* target = *request;
    void** vtbl = CSF_AT(void**, target, 0);

    // Flag byte at request+4: nonzero selects the upper flag value.
    int flags = 8 * (*(reinterpret_cast<uint8_t*>(request) + 4) != 0) + 2;

    int tableEntryField4 = GfxObjectProps_ReadTableEntryField4(reinterpret_cast<int>(self));

    if (reinterpret_cast<GfxAcceptVirtualFn>(vtbl[28])(target, nullptr, props, tableEntryField4 + 4, flags))
    {
        RpWorldSectorForAllAtomics(reinterpret_cast<int>(self),
                                   reinterpret_cast<GfxAtomicVisitorFn>(CSF_ADDR_FN(0x006BC220)),
                                   static_cast<int>(reinterpret_cast<uintptr_t>(request)));
    }
    return self;
}

HOOK(0x006BC8F0, GfxEnvironment_VisitAtomicsIfAccepted, "auto");
