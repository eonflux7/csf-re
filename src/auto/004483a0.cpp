#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall ClearElementArray(void* self, void* /*edx*/)
{
    auto* elements = CSF_AT(uint8_t*, self, 0x0);
    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;

    if (elements)
    {
        const int32_t count = CSF_AT(int32_t, elements, -4);
        auto* end = elements + static_cast<uint32_t>(count) * 16u;
        const int32_t lastIndex =
            static_cast<int32_t>(static_cast<uint32_t>(count) - 1u);
        if (lastIndex >= 0)
        {
            uint32_t remaining = static_cast<uint32_t>(count);
            do
            {
                end -= 0x10;
                --remaining;
                CSF_AT(void**, end, 0x0) = CSF_VTBL(0x0082D410);
            } while (remaining);
        }
        Mem_Free(elements - 4);
        CSF_AT(uint8_t*, self, 0x0) = nullptr;
    }
}

HOOK(0x004483A0, ClearElementArray, "auto");
