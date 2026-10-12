#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: C_008319c0_GetSubObjectField74 @ 0x004760A0 (__fastcall, no stack args, ret 0)
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void* /*edx*/));

// Returns 1 when the HUD is suspended: flag 0x40 set in the byte at +0x2E0, and the
// sub-object returned by the vtable slot 0xD8 reports field-74 value 2. Otherwise 0.
static char __fastcall IsHudSuspended(C_00833908* self, void* /*edx*/)
{
    // Byte flag at +0x2E0 (not a declared field of C_00833908).
    uint8_t flagByte = CSF_AT(uint8_t, self, 0x2E0);
    char result = 0;

    if ((flagByte & 0x40) == 0)
        return 0;

    // Object at +0x44 is a separate instance; slot 0xD8 / 4 = 54 of its vtable.
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0044));
    C_008319c0* subObject =
        reinterpret_cast<C_008319c0*>(
            ((void* (__fastcall*)(void*, void*))(CSF_AT(void**, owner, 0)[0xD8 / 4]))(owner, nullptr));

    if (subObject == nullptr)
        return 0;

    result = (C_008319c0_GetSubObjectField74(subObject, nullptr) == 2) ? 1 : 0;
    return result;
}

HOOK(0x495520, IsHudSuspended, "auto");
