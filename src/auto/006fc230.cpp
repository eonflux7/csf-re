#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when every bit set in a2 is also set in this->field_0040
// (a2 is a subset of the object's flag mask), otherwise 0.
static int __fastcall C_00862e88_IsMaskSubset(C_00862e88* self, void* /*edx*/, int mask) {
    return mask == (mask & static_cast<int>(self->field_0040));
}

HOOK(0x6FC230, C_00862e88_IsMaskSubset, "auto");
