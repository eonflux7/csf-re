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

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Destroys every RW texture in the table owned by this object, frees the
// table block, and clears the table fields.
static int __fastcall C_00861c08_ReleaseTextureTable(C_00861c08* self, void* /*edx*/)
{
    uint32_t i = 0;
    for (;; ++i) {
        uint32_t tableBegin = self->field_0034;
        if (tableBegin == 0) {
            break;
        }
        int32_t byteSpan = static_cast<int32_t>(self->field_0038 - tableBegin);
        uint32_t slotCount = static_cast<uint32_t>(byteSpan >> 2);
        if (i >= slotCount) {
            break;
        }
        int* slots = reinterpret_cast<int*>(static_cast<uintptr_t>(tableBegin));
        int texture = slots[i];
        if (texture != 0) {
            RwTextureDestroy(texture);
        }
    }

    int result = static_cast<int>(self->field_0034);
    if (result != 0) {
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0034)));
    }

    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    return result;
}

HOOK(0x6E60A0, C_00861c08_ReleaseTextureTable, "auto");
