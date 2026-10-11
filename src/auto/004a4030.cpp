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

// Game heap free used for field_006c (Mem_FreeDebug at 0x690E60).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Game heap free used for field_0050 (Mem_Free at 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the two owned buffers of C_008344f0 and restores the base C_0082d410 vtables.
static int __fastcall C_008344f0_ReleaseOwnedBuffers(C_008344f0* self, void* /*edx*/) {
    // Install this class's vtables first (as the original does).
    self->vtbl = CSF_VTBL(0x008344F0);
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x008344D0);

    if (self->field_006c) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_006c));
    }
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;

    int result = static_cast<int>(self->field_0050);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0050));
        self->field_0050 = 0;
    }

    // Restore the base class vtables.
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004A4030, C_008344f0_ReleaseOwnedBuffers, "auto");
