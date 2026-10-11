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

struct C_008408c0;

// Callees, bound by address.
CSF_FUNC(int, __cdecl, MenuSaveGame_ReleaseSlotBuffer, 0x005129A0,
         (uint32_t slotBuffer, uint32_t slotCount, void* slotOwnerSubobject, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Destructor-like teardown for C_008408c0: releases the slot buffer owned by the
// embedded subobject at this+8, clears its three fields and restores the base vtable.
static int __fastcall C_008408c0_ReleaseSlotBufferAndReset(C_008408c0* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008408C0);

    uint8_t* slotSubobject = reinterpret_cast<uint8_t*>(self) + 8;

    int result = CSF_AT(int, self, 0x0C);
    if (result != 0) {
        MenuSaveGame_ReleaseSlotBuffer(
            CSF_AT(uint32_t, self, 0x0C),
            CSF_AT(uint32_t, self, 0x10),
            slotSubobject,
            nullptr);

        result = CSF_AT(int, self, 0x0C);
        if (result != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
        }
    }

    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F4BC);
    return result;
}

HOOK(0x513B10, C_008408c0_ReleaseSlotBufferAndReset, "auto");
