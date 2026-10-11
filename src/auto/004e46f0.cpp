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

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys a C_0083e7d8 object: releases its clump-manager handle, restores the
// base/own vtables, and frees the object when bit 0 of a2 is set.
// Returns the object pointer (this).
static uint32_t* __fastcall C_0083e7d8_Destroy(C_0083e7d8* self, void* /*edx*/, char a2)
{
    // Temporarily point at the C_0083e7d8 vtable and the base-class data vtable.
    *(void***)self = CSF_VTBL(0x0083E7D8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0083E7BC));

    if (self->field_0038)
    {
        int globalIntValue = GetGlobalInt();
        int clumpManager = ResolveClumpManager(globalIntValue, nullptr);
        // Virtual slot 0x2C / 4 = 11 on the clump manager object.
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, clumpManager, 0)[11])(
            reinterpret_cast<void*>(clumpManager), nullptr, self->field_0038);
    }

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0083E144));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0082D410));
    *(void***)self = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4E46F0, C_0083e7d8_Destroy, "auto");
