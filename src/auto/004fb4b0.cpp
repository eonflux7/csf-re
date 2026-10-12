#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, DevConsole_RemoveRegistration, 0x00712CE0, (void* self, void* edx, int a2));

static int __fastcall C_0083f2f0_Destroy(C_0083f2f0* self, void* /*edx*/)
{
    const uint8_t registered = self->field_0010;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F2F0);

    if (registered)
    {
        const int globalState = GetGlobalState_50D190();
        DevConsole_RemoveRegistration(
            CSF_AT(void*, reinterpret_cast<void*>(globalState), 0x10),
            nullptr,
            static_cast<int>(self->field_000c));
    }

    uint32_t buffer = self->field_016c;
    self->field_0170 = 0;
    self->field_0174 = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_016c = 0;
    }

    buffer = self->field_017c;
    self->field_0180 = 0;
    self->field_0184 = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_017c = 0;
    }

    buffer = self->field_0148;
    self->field_014c = 0;
    self->field_0150 = 0;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_0148 = 0;
    }

    if (self->field_01c4 >= 0x10 && self->field_01b0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_01b0));

    self->field_01c4 = 15;
    self->field_01c0 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_01b0) = 0;

    buffer = self->field_0194;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_0194 = 0;
    }

    buffer = self->field_017c;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_017c = 0;
    }

    buffer = self->field_016c;
    if (buffer)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_016c = 0;
    }

    buffer = self->field_0148;
    int result = static_cast<int>(buffer);
    if (buffer)
    {
        result = Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_0148 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00830438);
    return result;
}

HOOK(0x004FB4B0, C_0083f2f0_Destroy, "auto");
