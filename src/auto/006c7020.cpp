#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_008608e8_ResetBaseFields, 0x006C6E60, (void* self, void* edx));

static int __fastcall C_008608e8_ReleaseChildrenAndClear(C_008608e8* self, void* /*edx*/)
{
    // Release every entry of the child pointer array (virtual slot 1, arg 1), clearing each slot.
    for (uint32_t i = 0; i < self->field_0024; ++i)
    {
        uint32_t element = reinterpret_cast<uint32_t*>(self->field_001c)[i];
        if (element)
        {
            void* child = reinterpret_cast<void*>(element);
            auto releaseFn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child, 0)[1]);
            releaseFn(child, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_001c)[i] = 0;
        }
    }

    uint32_t arrayBlock = self->field_001c;
    self->field_0020 = 0;
    self->field_0024 = 0;
    if (arrayBlock)
    {
        Mem_Free(reinterpret_cast<void*>(arrayBlock));
        self->field_001c = 0;
    }

    int result = C_008608e8_ResetBaseFields(self, nullptr);
    self->field_000c = 0;
    return result;
}

HOOK(0x006C7020, C_008608e8_ReleaseChildrenAndClear, "auto");
