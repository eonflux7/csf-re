#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, MissionScript_DestroyStringRange, 0x00697A80, (int a1, int a2));

static int __fastcall CoreFiles_ClearStorage(void* self, void* /*edx*/)
{
    const int firstString = CSF_AT(int, self, 0x54);
    if (firstString)
    {
        MissionScript_DestroyStringRange(firstString, CSF_AT(int, self, 0x58));

        void* storage = CSF_AT(void*, self, 0x54);
        if (storage)
            Mem_FreeDebug(storage);
    }

    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;

    int* firstElement = CSF_AT(int*, self, 0x34);
    if (firstElement)
    {
        MissionScript_DestroyElementRange(firstElement, CSF_AT(int*, self, 0x38));

        void* storage = CSF_AT(void*, self, 0x34);
        if (storage)
            Mem_FreeDebug(storage);
    }

    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;

    void* buffer = CSF_AT(void*, self, 0x20);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(buffer));
    if (buffer)
    {
        result = Mem_Free(buffer);
        CSF_AT(uint32_t, self, 0x20) = 0;
    }

    if (CSF_AT(uint32_t, self, 0x1C) >= 0x10u)
    {
        void* stringStorage = CSF_AT(void*, self, 0x08);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(stringStorage));
        if (stringStorage)
            result = Mem_FreeDebug(stringStorage);
    }

    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 0x08) = 0;
    return result;
}

HOOK(0x00698010, CoreFiles_ClearStorage, "auto");
