#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the 32-bit clump count stored at this + 0x34 + category * 16.
// Original: shl eax, 4 / mov eax, [eax + ecx + 0x34]; ret 4
static int __fastcall C_00833708_GetCategoryClumpCount(C_00833708* self, void* /*edx*/, int category) {
    return CSF_AT(int, self, 0x34 + category * 16);
}

HOOK(0x006BCA20, C_00833708_GetCategoryClumpCount, "auto");
