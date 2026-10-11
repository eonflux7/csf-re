#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void Game_DestroyElementArray(uint8_t* elements)
{
    const uint32_t count = CSF_AT(uint32_t, elements, -4);
    if (static_cast<int32_t>(count - 1) >= 0)
    {
        uint8_t* cursor = elements + count * 16 + 4;
        uint32_t remaining = count;
        do
        {
            cursor -= 16;
            --remaining;
            CSF_AT(void**, cursor, 0) = CSF_VTBL(0x0082D410);
        } while (remaining);
    }

    Mem_Free(elements - 4);
}

static void __fastcall Game_ClearElementArrays(void* self, void* /*edx*/)
{
    uint8_t* elements = CSF_AT(uint8_t*, self, 0x20);
    if (elements)
    {
        Game_DestroyElementArray(elements);
        CSF_AT(uint8_t*, self, 0x20) = nullptr;
    }

    elements = CSF_AT(uint8_t*, self, 0x10);
    if (elements)
    {
        Game_DestroyElementArray(elements);
        CSF_AT(uint8_t*, self, 0x10) = nullptr;
    }

    elements = CSF_AT(uint8_t*, self, 0x0);
    if (elements)
    {
        Game_DestroyElementArray(elements);
        CSF_AT(uint8_t*, self, 0x0) = nullptr;
    }
}

HOOK(0x00446150, Game_ClearElementArrays, "auto");
