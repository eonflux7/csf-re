#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Releases every child effect held in the pointer array at field_0008 (each child's
// slot-0 virtual destructor is called with flag 1), then the lone object at field_0004,
// then the array itself.
static int __fastcall C_00867078_DestroyEffects(C_00867078* self, void* /*edx*/)
{
    uint32_t childCount = self->field_0010;
    uint32_t index = 0;

    self->vtbl = CSF_VTBL(0x00867078);

    for (; index < childCount; ++index) {
        uint32_t slotAddress = self->field_0008 + 4u * index;
        void** child = *reinterpret_cast<void***>(slotAddress);
        if (child != nullptr) {
            void* deleteFn = CSF_AT(void**, child, 0)[0];
            ((void(__fastcall*)(void*, void*, int))deleteFn)(child, nullptr, 1);
            *reinterpret_cast<uint32_t*>(self->field_0008 + 4u * index) = 0;
        }
    }

    void** owned = reinterpret_cast<void**>(self->field_0004);
    if (owned != nullptr) {
        void* deleteFn = CSF_AT(void**, owned, 0)[0];
        ((void(__fastcall*)(void*, void*, int))deleteFn)(owned, nullptr, 1);
        self->field_0004 = 0;
    }

    int result = static_cast<int>(self->field_0008);
    if (result != 0) {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }
    return result;
}

HOOK(0x007318F0, C_00867078_DestroyEffects, "auto");
