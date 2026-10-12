#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall AppConfig_DestroyStorage(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x104) >= 0x10u &&
        CSF_AT(void*, self, 0xF0))
        Mem_FreeDebug(CSF_AT(void*, self, 0xF0));
    CSF_AT(uint32_t, self, 0x104) = 15;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint8_t, self, 0xF0) = 0;

    if (CSF_AT(uint32_t, self, 0xE4) >= 0x10u &&
        CSF_AT(void*, self, 0xD0))
        Mem_FreeDebug(CSF_AT(void*, self, 0xD0));
    CSF_AT(uint32_t, self, 0xE4) = 15;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint8_t, self, 0xD0) = 0;

    if (CSF_AT(uint32_t, self, 0xA4) >= 0x10u &&
        CSF_AT(void*, self, 0x90))
        Mem_FreeDebug(CSF_AT(void*, self, 0x90));
    CSF_AT(uint32_t, self, 0xA4) = 15;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint8_t, self, 0x90) = 0;

    if (CSF_AT(uint32_t, self, 0x84) >= 0x10u &&
        CSF_AT(void*, self, 0x70))
        Mem_FreeDebug(CSF_AT(void*, self, 0x70));
    CSF_AT(uint32_t, self, 0x84) = 15;
    CSF_AT(uint32_t, self, 0x80) = 0;
    CSF_AT(uint8_t, self, 0x70) = 0;

    if (CSF_AT(uint32_t, self, 0x68) >= 0x10u &&
        CSF_AT(void*, self, 0x54))
        Mem_FreeDebug(CSF_AT(void*, self, 0x54));
    CSF_AT(uint32_t, self, 0x68) = 15;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint8_t, self, 0x54) = 0;

    int* first = CSF_AT(int*, self, 0x38);
    if (first)
    {
        int* last = CSF_AT(int*, self, 0x3C);
        MissionScript_DestroyElementRange(first, last);
        if (CSF_AT(void*, self, 0x38))
            Mem_FreeDebug(CSF_AT(void*, self, 0x38));
    }
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;

    if (CSF_AT(uint32_t, self, 0x2C) >= 0x10u &&
        CSF_AT(void*, self, 0x18))
        Mem_FreeDebug(CSF_AT(void*, self, 0x18));
    CSF_AT(uint32_t, self, 0x2C) = 15;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint8_t, self, 0x18) = 0;

    void* storage = CSF_AT(void*, self, 0x04);
    int result = 0;
    if (storage)
        result = Mem_FreeDebug(storage);
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return result;
}

HOOK(0x0075FE80, AppConfig_DestroyStorage, "auto");
