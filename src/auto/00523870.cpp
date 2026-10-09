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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a C_00840ff0 buffer holder: frees the heap block if the inline
// small-buffer threshold (16) was exceeded, then restores the empty state.
static int __fastcall C_00840ff0_ResetBuffer(C_00840ff0* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x00840FF0);

    int result = 0;
    if (self->field_0058 >= 0x10) {
        result = (int)self->field_0044;
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0044));
        }
    }

    self->field_0058 = 15;
    self->field_0054 = 0;
    CSF_AT(uint8_t, self, 0x44) = 0;
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00523870, C_00840ff0_ResetBuffer, "auto");
