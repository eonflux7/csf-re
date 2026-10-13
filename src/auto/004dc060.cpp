#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Resets the TAiState object to its initial state and returns the object's own address.
static uint32_t* __fastcall C_0083dfa0_ResetAiBaseState(C_0083dfa0* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0083E050);
    self->vtbl = CSF_VTBL(0x0083DFA0);
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083DF18));

    self->field_0008 = static_cast<uint32_t>(CSF_ADDR_DATA(0x0083E030));

    CSF_AT(uint32_t, self, 0xC) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 10;

    CSF_AT(uint32_t, self, 0xC) = reinterpret_cast<uint32_t>(&self->field_0008);
    CSF_AT(uint32_t, self, 0x10) = reinterpret_cast<uint32_t>(&self->field_0008);
    CSF_AT(uint32_t, self, 0x18) = reinterpret_cast<uint32_t>(&self->field_0004);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004DC060, C_0083dfa0_ResetAiBaseState, "auto");
