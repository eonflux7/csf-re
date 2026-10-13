#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"

// Stores a bounding box as two vectors (min at +0x00, max at +0x0C) and returns the
// second Y-like argument, as the original does.
//   min = (a2, a4, a6), max = (a3, a5, a7)
static int __fastcall BBoxMath_SetBounds(void* self, void* /*edx*/,
                                         int a2, int a3, int a4, int a5, int a6, int a7) {
    CSF_AT(int, self, 0x0C) = a3;
    CSF_AT(int, self, 0x10) = a5;
    CSF_AT(int, self, 0x14) = a7;
    CSF_AT(int, self, 0x00) = a2;
    CSF_AT(int, self, 0x04) = a4;
    CSF_AT(int, self, 0x08) = a6;
    return a4;
}

HOOK(0x006AB4E0, BBoxMath_SetBounds, "auto");
