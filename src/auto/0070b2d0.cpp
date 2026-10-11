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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static void __fastcall C_008636fc_ReleaseOwnedObjects(C_008636fc* self, void* /*edx*/)
{
    uint32_t count = self->field_000c;
    self->vtbl = CSF_VTBL(0x008636FC);

    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0004)[i];
        if (entry != 0)
        {
            void* child = reinterpret_cast<void*>(entry);
            // slot +0x4 of the child's vtable (index 1), called with argument 1
            ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, child, 0)[1]))(child, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0004)[i] = 0;
        }
    }

    uint32_t extra = self->field_001c;
    if (extra != 0)
    {
        void* extraObj = reinterpret_cast<void*>(extra);
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, extraObj, 0)[1]))(extraObj, nullptr, 1);
        self->field_001c = 0;
    }

    if (self->field_0004 != 0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x0070B2D0, C_008636fc_ReleaseOwnedObjects, "auto");
