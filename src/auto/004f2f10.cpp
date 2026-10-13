#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, HudStringShow_ProcessPair, 0x004F23A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Cleanup for the HUD string-show object: releases the owned string buffer
// (field_003c) and the pair block at field_0008, resets the state fields,
// and leaves the object with the base class vtable.
static int __fastcall C_0083f0ec_Cleanup(C_0083f0ec* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083F0EC);

    if (CSF_AT(uint32_t, self, 0x50) >= 8 && self->field_003c != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_003c)));

    self->field_0050 = 7;
    self->field_004c = 0;
    self->field_003c &= 0xFFFF0000u; // original writes only the low word (mov word ptr)

    int result = static_cast<int>(self->field_0008);
    if (result != 0)
    {
        // The original pushes four values: ecx (this), esi (this+4),
        // edx (field_000c) and eax (field_0008), in that order.
        HudStringShow_ProcessPair(
            result,
            static_cast<int>(self->field_000c),
            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 4)),
            static_cast<int>(reinterpret_cast<uintptr_t>(self)));

        result = static_cast<int>(self->field_0008);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)));
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004F2F10, C_0083f0ec_Cleanup, "auto");
