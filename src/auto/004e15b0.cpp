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

// Frees a block allocated by the game heap (cdecl, pops nothing).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the object's owned child: if the child's ref-flag is set, calls its
// vtable slot 0 with arg 3 (release-and-delete); otherwise frees the raw block
// at (child - 4). Then clears the child pointer and restores the vtables.
static int __fastcall C_0083e248_ResetAndReleaseChild(C_0083e248* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0083E248);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083E22C));

    int result = 0;
    void* child = reinterpret_cast<void*>(self->field_0034);
    if (child) {
        int refFlag = CSF_AT(int, child, -4);
        if (refFlag != 0) {
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0])(child, nullptr, 3);
        } else {
            void* rawBlock = CSF_AT(void*, child, -4);
            result = Mem_Free(rawBlock);
        }
        self->field_0034 = 0;
    }

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083E144));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x4E15B0, C_0083e248_ResetAndReleaseChild, "auto");
