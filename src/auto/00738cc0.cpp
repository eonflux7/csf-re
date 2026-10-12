#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall: ecx = this, one stack argument (pair pointer), callee pops 4 bytes.
// Updates the cached (field_0014, field_0018) pair from the two dwords at `pair`
// when either differs. Returns `pair` itself when unchanged, otherwise the new
// value of pair[1] (as the original's eax does).
static uint32_t* __fastcall C_008673b0_UpdateSecondaryCachedPair(C_008673b0* self, void* /*edx*/, uint32_t* pair) {
    uint32_t* result = pair;
    if (self->field_0014 != pair[0] || self->field_0018 != pair[1]) {
        self->field_0014 = pair[0];
        result = reinterpret_cast<uint32_t*>(pair[1]);
        self->field_0018 = reinterpret_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x738CC0, C_008673b0_UpdateSecondaryCachedPair, "auto");
