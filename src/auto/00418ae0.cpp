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

// Base-class release (thunk j_C_0082deb8_ReleaseChildBase -> 0x40D170).
CSF_FUNC(int, __fastcall, sub_40D170, 0x0040D170, (void* self, void* edx));
// Tail-jumped finisher (0x40F760); returns its result to our caller.
CSF_FUNC(int, __fastcall, sub_40F760, 0x0040F760, (void* self, void* edx));
// Game heap free (cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_0082e1e8_Destroy(C_0082e1e8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0082E1E8);

    sub_40D170(self, nullptr);

    if (self->field_0054) {
        Mem_Free(reinterpret_cast<void*>(self->field_0054));
        self->field_0054 = 0;
    }
    if (self->field_0044) {
        Mem_Free(reinterpret_cast<void*>(self->field_0044));
        self->field_0044 = 0;
    }
    if (self->field_0034) {
        Mem_Free(reinterpret_cast<void*>(self->field_0034));
        self->field_0034 = 0;
    }

    return sub_40F760(self, nullptr);
}

HOOK(0x00418AE0, C_0082e1e8_Destroy, "auto");
