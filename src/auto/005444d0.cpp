#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MenuSaveGame_ReleaseElements, 0x00544340,
         (int* first, int* last, void* container, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int* __fastcall MenuSaveGame_ClearStorage(void* self, void* /*edx*/)
{
    if (CSF_AT(std::uint32_t, self, 0x2C) >= 8u)
    {
        void* block = CSF_AT(void*, self, 0x18);
        if (block)
            Mem_FreeDebug(block);
    }

    CSF_AT(std::uint32_t, self, 0x2C) = 7;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint16_t, self, 0x18) = 0;

    int* result = CSF_AT(int*, self, 0x08);
    void* container = static_cast<std::uint8_t*>(self) + 4;
    if (result)
    {
        MenuSaveGame_ReleaseElements(
            result, CSF_AT(int*, container, 0x08), container, self);

        result = CSF_AT(int*, container, 0x04);
        if (result)
        {
            result = reinterpret_cast<int*>(
                static_cast<std::uintptr_t>(Mem_FreeDebug(result)));
        }
    }

    CSF_AT(std::uint32_t, container, 0x04) = 0;
    CSF_AT(std::uint32_t, container, 0x08) = 0;
    CSF_AT(std::uint32_t, container, 0x0C) = 0;
    return result;
}

HOOK(0x005444D0, MenuSaveGame_ClearStorage, "auto");
