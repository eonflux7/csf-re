#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at index a3 of the row referenced by entry a2 of a
// 16-byte-stride table embedded in `this` (table starts at +0x2C).
static int __fastcall C_00833708_GetCategoryClump(C_00833708* self, void* /*edx*/, int a2, int a3) {
    uint32_t rowAddress = CSF_AT(uint32_t, self, 0x2C + a2 * 16);
    int* row = reinterpret_cast<int*>(rowAddress);
    return row[a3];
}

HOOK(0x6BCA40, C_00833708_GetCategoryClump, "auto");
