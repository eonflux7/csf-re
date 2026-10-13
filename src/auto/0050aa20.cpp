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

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

// Reads the float stored at byte offset `offset` inside the global value block.
// The block is fetched fresh on every call, matching the original's two separate lookups.
static float ReadGlobalFloat(int offset) {
    uint8_t* globals = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_75FDF0()));
    return CSF_AT(float, globals, offset);
}

// Resets an S_0050aa20 options record to its default values.
// Returns 1 (TRUE) to the caller.
static int __fastcall S_0050aa20_InitDefaults(S_0050aa20* self, void* /*edx*/) {
    self->field_000c = 2;
    self->field_0004 = static_cast<uint32_t>(static_cast<int64_t>(ReadGlobalFloat(0xB0)));
    self->field_0008 = static_cast<uint32_t>(static_cast<int64_t>(ReadGlobalFloat(0xB4)));
    self->field_0010 = 0x3F000000;
    self->field_0014 = 1;
    self->field_0015 = 1;
    self->field_0018 = 1;
    self->field_001c = 3;
    self->field_0020 = 1;
    self->field_0024 = 1;
    self->field_0028 = 1;
    self->field_002c = 1;
    self->field_0030 = 1;
    return 1;
}

HOOK(0x0050AA20, S_0050aa20_InitDefaults, "auto");
