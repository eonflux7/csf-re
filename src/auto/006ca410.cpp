#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies three flag bytes and two dwords from a descriptor (a2) into the render-target object.
// Returns the descriptor pointer unchanged (the original leaves it in eax).
static int __fastcall C_00860a30_SetupRenderTargetFromDesc(C_00860a30* self, void* /*edx*/, int a2)
{
    uint8_t* desc = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));

    self->field_000c = 1;

    uint32_t flags = self->field_0014;
    if (CSF_AT(uint8_t, desc, 0x44))
        flags |= 2u;
    else
        flags &= ~2u;
    self->field_0014 = flags;

    if (CSF_AT(uint8_t, desc, 0x43))
        self->field_0014 |= 1u;
    else
        self->field_0014 &= ~1u;

    flags = self->field_0014;
    if (CSF_AT(uint8_t, desc, 0x42))
        flags |= 4u;
    else
        flags &= ~4u;
    self->field_0014 = flags;

    self->field_0010 = CSF_AT(uint32_t, desc, 0x3c);
    self->field_0018 = static_cast<uint32_t>(a2);

    return a2;
}

HOOK(0x006CA410, C_00860a30_SetupRenderTargetFromDesc, "auto");
