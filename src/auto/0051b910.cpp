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

// Releases the buffer owned by the C_0086f128 base; tail-jumped from the cleanup below.
CSF_FUNC(int, __fastcall, ReleaseOwnedBuffer, 0x00762F50, (C_0086f128* self, void*));

// Resets the object's vtable to C_00840e0c's, then tail-calls the base buffer release.
static int __fastcall C_00840E0C_Cleanup(C_00840e0c* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840E0C);
    return ReleaseOwnedBuffer(reinterpret_cast<C_0086f128*>(self), nullptr);
}

HOOK(0x0051B910, C_00840E0C_Cleanup, "auto");
