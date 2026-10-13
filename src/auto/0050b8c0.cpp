#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall COptionsFile_Destroy(COptionsFile* self, void* /*edx*/)
{
    int result = 0;
    void* object = reinterpret_cast<void*>(self->field_0004);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083FFA8);

    if (object)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0004 = 0;
    }

    object = reinterpret_cast<void*>(self->field_0008);
    if (object)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0008 = 0;
    }

    object = reinterpret_cast<void*>(self->field_000c);
    if (object)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_000c = 0;
    }

    object = reinterpret_cast<void*>(self->field_0010);
    if (object)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0010 = 0;
    }

    if (self->field_004c >= 8)
    {
        result = static_cast<int>(self->field_0038);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0038));
    }
    self->field_004c = 7;
    self->field_0048 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_0038) = 0;

    if (self->field_0030 >= 8)
    {
        result = static_cast<int>(self->field_001c);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_001c));
    }
    self->field_002c = 0;
    self->field_0030 = 7;
    *reinterpret_cast<uint16_t*>(&self->field_001c) = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083FDF4);
    return result;
}

HOOK(0x0050B8C0, COptionsFile_Destroy, "auto");
