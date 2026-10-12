#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_43E6C0, 0x0043E6C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, sub_43EDB0, 0x0043EDB0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int*, __fastcall, sub_43F780, 0x0043F780, (uint32_t* a1, int a2, int* a3, int a4, int a5));
CSF_FUNC(int, __fastcall, Sub_43F890, 0x0043F890, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00831008_Destroy(C_00831008* self, void* /*edx*/)
{
    alignas(8) uint8_t iteratorBytes[0x4] = {};
    int* iteratorResult = reinterpret_cast<int*>(iteratorBytes);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00831008);
    sub_43E6C0(self, nullptr);
    Sub_43F890(reinterpret_cast<uint8_t*>(self) + 0x30, nullptr);

    uint32_t storage = self->field_0024;
    if (storage)
    {
        sub_43EDB0(
            static_cast<int>(storage),
            CSF_AT(int, self, 0x28),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x20),
            0);

        storage = self->field_0024;
        if (storage)
            Mem_FreeDebug(reinterpret_cast<void*>(storage));
    }

    self->field_0024 = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;

    uint32_t sentinel = self->field_0014;
    int firstNode = CSF_AT(int, reinterpret_cast<void*>(sentinel), 0);
    sub_43F780(
        &CSF_AT(uint32_t, self, 0x10),
        0,
        iteratorResult,
        firstNode,
        static_cast<int>(sentinel));

    storage = self->field_0014;
    if (storage)
        Mem_FreeDebug(reinterpret_cast<void*>(storage));

    self->field_0014 = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;

    sentinel = self->field_0008;
    firstNode = CSF_AT(int, reinterpret_cast<void*>(sentinel), 0);
    sub_43F780(
        &self->field_0004,
        0,
        iteratorResult,
        firstNode,
        static_cast<int>(sentinel));

    storage = self->field_0008;
    int result = static_cast<int>(storage);
    if (storage)
        result = Mem_FreeDebug(reinterpret_cast<void*>(storage));

    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00441160, C_00831008_Destroy, "auto");
