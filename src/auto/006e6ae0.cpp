#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall with 2 stack args (a2, a3); callee pops 8 bytes.
static int __fastcall C_00861d48_LoadFromRecord(C_00861d48* self, void* /*edx*/, int a2, uint32_t* a3) {
    self->field_0008 = static_cast<uint32_t>(a2);

    CSF_AT(uint32_t, self, 0x0C) = a3[0];
    CSF_AT(uint32_t, self, 0x10) = a3[1];
    CSF_AT(uint32_t, self, 0x14) = a3[2];

    CSF_AT(uint32_t, self, 0x18) = a3[11];
    CSF_AT(uint32_t, self, 0x1C) = a3[12];
    CSF_AT(uint32_t, self, 0x20) = a3[13];

    uint32_t value = a3[14];
    self->field_0004 = value;
    return static_cast<int>(value);
}

HOOK(0x006E6AE0, C_00861d48_LoadFromRecord, "auto");
