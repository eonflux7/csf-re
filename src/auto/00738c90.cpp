#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall: ecx = this, one stack argument (pair pointer), callee pops 4 bytes.
// Updates the cached (field_000c, field_0010) pair from the two dwords at `pair`
// when either differs. Returns `pair` itself when unchanged, otherwise the new
// value of pair[1] (as the original's eax does).
static uint32_t* __fastcall C_008673b0_UpdateCachedPair(C_008673b0* self, void* /*edx*/, uint32_t* pair) {
    uint32_t* result = pair;
    if (self->field_000c != pair[0] || self->field_0010 != pair[1]) {
        self->field_000c = pair[0];
        result = reinterpret_cast<uint32_t*>(pair[1]);
        self->field_0010 = reinterpret_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x738C90, C_008673b0_UpdateCachedPair, "auto");
