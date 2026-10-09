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

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the entry at 'index' from the slot array of CCharWeaponFsm: releases the
// object stored there (vtable slot 0, flag 1), shifts the following entries down
// by one slot, and shrinks the array end pointer by one slot.
static int __fastcall CCharWeaponFsm_RemoveSlotAndShift(CCharWeaponFsm* self, void* /*edx*/, int index)
{
    int32_t count = static_cast<int32_t>(self->field_0024);
    if (count <= index)
    {
        if (count == index)
            self->vSlotF8(-1);
    }
    else
    {
        self->field_0024 = static_cast<uint32_t>(count - 1);
    }

    uint32_t byteOffset = static_cast<uint32_t>(index) * 4u;
    uintptr_t slotAddr = static_cast<uintptr_t>(self->field_0018) + byteOffset;

    void* slotObject = *reinterpret_cast<void**>(slotAddr);
    if (slotObject)
    {
        // Virtual slot 0 of the stored object, called with flag 1 (thiscall).
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotObject, 0)[0])(slotObject, nullptr, 1);
        *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0018) + byteOffset) = 0;
    }

    // Shift the remaining slots down by one entry.
    slotAddr = static_cast<uintptr_t>(self->field_0018) + byteOffset;
    int32_t bytesRemaining = static_cast<int32_t>(self->field_001c) - static_cast<int32_t>(slotAddr + 4);
    std::size_t byteCount = static_cast<std::size_t>((bytesRemaining >> 2) << 2);
    _memcpy(reinterpret_cast<void*>(slotAddr), reinterpret_cast<const void*>(slotAddr + 4), byteCount);

    int result = static_cast<int>(self->field_001c) - 4;
    self->field_001c = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x00481840, CCharWeaponFsm_RemoveSlotAndShift, "auto");
