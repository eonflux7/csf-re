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

// __thiscall(this, char a2, char a3), callee pops 8 bytes (ret 8).
// Stores a2 at +0x6B and a3 at +0x6C, returns a2.
static char __fastcall C_00860bb8_SetByteFlagPair(C_00860bb8* self, void* /*edx*/, char flagA, char flagB) {
    self->field_006b = static_cast<uint8_t>(flagA);
    self->field_006c = static_cast<uint8_t>(flagB);
    return flagA;
}

HOOK(0x006CABE0, C_00860bb8_SetByteFlagPair, "auto");
