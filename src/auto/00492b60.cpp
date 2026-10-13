#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stores a2 at this+0x1E8 and a3 at this+0x1EC, clamping the stored value to 100.
// Returns the unclamped a3.
static int __fastcall C_00833908_AssignDefinitionLink(C_00833908* self, void* /*edx*/, int a2, int a3) {
    CSF_AT(int, self, 0x1E8) = a2;
    int result = a3;
    CSF_AT(int, self, 0x1EC) = a3;
    if (a3 > 100) {
        CSF_AT(int, self, 0x1EC) = 100;
    }
    return result;
}

HOOK(0x492B60, C_00833908_AssignDefinitionLink, "auto");
