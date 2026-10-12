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
#include "legacy/string.h"

// Forwards (a2, a3) to the virtual slot +0x34 of C_00867358, passing a
// zero-initialised 8-byte scratch pair as the fourth argument.
static int __fastcall C_00867358_InvokeSlot34WithScratchPair(C_00867358* self, void* /*edx*/, int a2, int a3) {
    alignas(8) uint32_t scratchPair[2];
    scratchPair[0] = 0;
    scratchPair[1] = 0;

    return self->vSlot34(reinterpret_cast<uint32_t*>(a2),
                         reinterpret_cast<float*>(a3),
                         scratchPair);
}

HOOK(0x7387B0, C_00867358_InvokeSlot34WithScratchPair, "auto");
