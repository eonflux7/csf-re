#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original Mem_FreeDebug (__cdecl, pops its own argument; caller does add esp, 4)
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the two owned heap buffers of a C_008449a0 (team colour / sabotage bomb data),
// resets the bookkeeping fields and restores the base-class vtable.
// Mirrors the destructor body at 0x0055AE60 (__thiscall, ret 0).
static int __fastcall C_008449a0_ReleaseOwnedBuffers(C_008449a0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008449A0);

    // Large-string style buffer (capacity >= 16 means heap-allocated).
    if (self->field_0158 >= 0x10 && self->field_0144) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0144));
    }
    self->field_0158 = 15;
    self->field_0154 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0144) = 0;

    int result = static_cast<int>(self->field_000c);
    if (result) {
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));
    }
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    self->vtbl = CSF_VTBL(0x008446CC);
    return result;
}

HOOK(0x0055AE60, C_008449a0_ReleaseOwnedBuffers, "auto");
