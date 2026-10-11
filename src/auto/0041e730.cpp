#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Resets a 0x1C-byte AI task state block to its initial values.
// Original: 0x41E730, __thiscall, ret 0. Returns the same object pointer.
static uint32_t* __fastcall AiTaskState_ResetFields(void* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(uint8_t, self, 0x01) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0041E730, AiTaskState_ResetFields, "auto");
