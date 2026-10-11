#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0083e9d0_Destroy, 0x004E5D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083eb70_ReleaseContents, 0x004E7390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall C_0083eb70_Destroy(C_0083eb70* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083EB70);
    self->field_0004 = reinterpret_cast<uintptr_t>(CSF_ADDR_VT(0x0083EB54));

    C_0083eb70_ReleaseContents(self, nullptr);

    void* inner = reinterpret_cast<void*>(self->field_0090);
    if (inner)
    {
        if (CSF_AT(int, inner, -4))
        {
            // scalar deleting destructor, flag 3
            void** vtbl = CSF_AT(void**, inner, 0);
            ((void(__fastcall*)(void*, void*, int))vtbl[0])(inner, nullptr, 3);
        }
        else
        {
            Mem_Free(reinterpret_cast<uint8_t*>(inner) - 4);
        }
        self->field_0090 = 0;
    }

    if (self->field_0080)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0080));
        self->field_0080 = 0;
    }
    if (self->field_0070)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0070));
        self->field_0070 = 0;
    }
    if (self->field_0060)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0060));
        self->field_0060 = 0;
    }
    if (self->field_0050)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0050));
        self->field_0050 = 0;
    }

    C_0083e9d0_Destroy(self, nullptr);
}

HOOK(0x004E7430, C_0083eb70_Destroy, "auto");
