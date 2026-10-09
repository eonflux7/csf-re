#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, DestroyElementRange, 0x00469760, (int* begin, int* end, int* owner, int* context));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, MissionScript_DestroyStringRange, 0x00697A80, (int a1, int a2));

static int __fastcall MissionScript_ClearOwnedData(void* self, void* /*edx*/)
{
    if (CSF_AT(void*, self, 0x34))
    {
        Mem_Free(CSF_AT(void*, self, 0x34));
        CSF_AT(void*, self, 0x34) = nullptr;
    }

    if (CSF_AT(void*, self, 0x24))
    {
        Mem_Free(CSF_AT(void*, self, 0x24));
        CSF_AT(void*, self, 0x24) = nullptr;
    }

    int* begin = CSF_AT(int*, self, 0x18);
    if (begin)
    {
        DestroyElementRange(
            begin,
            CSF_AT(int*, self, 0x1C),
            reinterpret_cast<int*>(static_cast<std::uint8_t*>(self) + 0x14),
            nullptr);

        if (CSF_AT(void*, self, 0x18))
            Mem_FreeDebug(CSF_AT(void*, self, 0x18));
    }

    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;

    int result = CSF_AT(int, self, 0x08);
    if (result)
    {
        MissionScript_DestroyStringRange(result, CSF_AT(int, self, 0x0C));
        result = CSF_AT(int, self, 0x08);
        if (result)
            result = Mem_FreeDebug(CSF_AT(void*, self, 0x08));
    }

    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    return result;
}

HOOK(0x0046ACF0, MissionScript_ClearOwnedData, "auto");
