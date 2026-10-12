#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads a 32-bit value at self+0x10 when self+0x08 is nonzero; otherwise the
// value is -1. Writes the value to out[0] and self to out[1], then returns
// whether the value is -1.
static bool __fastcall CursorReadIndexPair(void* self, void* /*edx*/, int* out) {
    int value;
    if (CSF_AT(uint32_t, self, 0x08) != 0) {
        value = CSF_AT(int, self, 0x10);
        out[1] = static_cast<int>(reinterpret_cast<uintptr_t>(self));
        out[0] = value;
    } else {
        out[0] = -1;
        value = out[0];
        out[1] = static_cast<int>(reinterpret_cast<uintptr_t>(self));
    }
    return value == -1;
}

HOOK(0x006DA400, CursorReadIndexPair, "auto");
