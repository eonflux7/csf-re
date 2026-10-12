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

// Copies the candidate record at src into self (thiscall, one stack arg, ret 4).
// Fields at 0x14, 0x24 and 0x28-skipped slots are not touched by the original.
// Returns self.
static uint32_t* __fastcall AiCore_CopyCandidate(void* self, void* /*edx*/, void* src) {
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, src, 0x04);
    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, src, 0x08);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, src, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, src, 0x10);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, src, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, src, 0x1C);
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, src, 0x20);
    CSF_AT(uint32_t, self, 0x24) = CSF_AT(uint32_t, src, 0x24);
    CSF_AT(uint32_t, self, 0x28) = CSF_AT(uint32_t, src, 0x28);
    CSF_AT(uint32_t, self, 0x2C) = CSF_AT(uint32_t, src, 0x2C);
    CSF_AT(uint32_t, self, 0x30) = CSF_AT(uint32_t, src, 0x30);
    return static_cast<uint32_t*>(self);
}

HOOK(0x4B8FD0, AiCore_CopyCandidate, "auto");
