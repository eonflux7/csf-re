#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class destructor, bound by address (same signature as the packet's bind line).
CSF_FUNC(unsigned int, __fastcall, C_00869cd0_Destroy_7563B0, 0x007563B0, (C_00869cd0* self, void*));

// Restores C_00840de4's vtable, then tail-calls the base-class cleanup and returns its result.
static int __fastcall C_008406d0_CleanupBase(C_00840de4* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0) = CSF_VTBL(0x00840DE4);
    return static_cast<int>(C_00869cd0_Destroy_7563B0(self, nullptr));
}

HOOK(0x0051B620, C_008406d0_CleanupBase, "auto");
