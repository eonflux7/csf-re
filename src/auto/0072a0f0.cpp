#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00863f60_Destroy, 0x00713F40,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* memory));


static C_00865a10* __fastcall C_00865a10_Destroy(
    C_00865a10* self, void* /*edx*/, char flags)
{
    void* texture = reinterpret_cast<void*>(self->field_0050);
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00865A10);

    if (texture)
        ReleaseObjectReference(static_cast<uint8_t*>(texture) + 4, nullptr);

    texture = reinterpret_cast<void*>(self->field_0054);
    if (texture)
        ReleaseObjectReference(static_cast<uint8_t*>(texture) + 4, nullptr);

    C_00863f60_Destroy(self, nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x0072A0F0, C_00865a10_Destroy, "auto");
