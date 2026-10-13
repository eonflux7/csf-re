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

static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases each object in the slot array (virtual slot 0 call with flag 1), then frees
// the auxiliary block and the slot array itself. Leaves the object as C_0082d410.
static int __fastcall C_008311f0_Destroy(C_008311f0* self, void* /*edx*/) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008311F0);

    uint32_t slotCount = self->field_000c;
    uint32_t slotIndex = 0;
    while (slotIndex < slotCount) {
        uint32_t* slotArray = reinterpret_cast<uint32_t*>(self->field_0004);
        void* slotObject = reinterpret_cast<void*>(slotArray[slotIndex]);
        if (slotObject) {
            // Virtual slot 0 (deleting-style destructor), flag 1.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotObject, 0)[0])(slotObject, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0004)[slotIndex] = 0;
        }
        ++slotIndex;
    }

    if (self->field_0014) {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    int result = static_cast<int>(self->field_0004);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0044DB40, C_008311f0_Destroy, "auto");
