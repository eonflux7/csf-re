#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

static int __fastcall C_008670f0_Destroy(C_008670f0* self, void* /*edx*/)
{
    const uint32_t reference = self->field_0014;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008670F0);

    if (reference)
        ReleaseObjectReference(reinterpret_cast<void*>(reference), nullptr);

    void* owner = reinterpret_cast<void*>(self->field_0004);
    reinterpret_cast<void(__fastcall*)(void*, void*, C_008670f0*)>(
        CSF_AT(void**, owner, 0)[8 / 4])(owner, nullptr, self);

    if (self->field_00dc)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00dc));
        self->field_00dc = 0;
    }
    if (self->field_0088)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0088));
        self->field_0088 = 0;
    }
    if (self->field_0078)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0078));
        self->field_0078 = 0;
    }
    if (self->field_0068)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0068));
        self->field_0068 = 0;
    }

    int result = static_cast<int>(self->field_0058);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0058));
        self->field_0058 = 0;
    }

    if (self->field_0054 >= 8)
    {
        result = static_cast<int>(self->field_0040);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0040));
    }
    self->field_0054 = 7;
    self->field_0050 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_0040) = 0;

    if (self->field_0038 >= 8)
    {
        result = static_cast<int>(self->field_0024);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0024));
    }
    self->field_0034 = 0;
    self->field_0038 = 7;
    *reinterpret_cast<uint16_t*>(&self->field_0024) = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008670CC);
    return result;
}

HOOK(0x007348F0, C_008670f0_Destroy, "auto");
