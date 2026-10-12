#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall DestroyElementArray(void* self, void* /*edx*/)
{
    auto* elements = CSF_AT(uint8_t*, self, 0);
    CSF_AT(int, self, 4) = 0;
    CSF_AT(int, self, 8) = 0;

    if (!elements)
        return 0;

    auto* block = elements - 4;
    const uint32_t count = CSF_AT(uint32_t, block, 0);
    auto* end = elements + count * 24u;

    if (static_cast<int32_t>(count - 1u) >= 0)
    {
        uint32_t remaining = count;
        do
        {
            end -= 0x18;
            --remaining;
            CSF_AT(void**, end, 0x0C) = CSF_VTBL(0x0082D410);
            CSF_AT(void**, end, 0) = CSF_VTBL(0x0082D410);
        }
        while (remaining);
    }

    const int result = Mem_Free(block);
    CSF_AT(uint8_t*, self, 0) = nullptr;
    return result;
}

HOOK(0x00448350, DestroyElementArray, "auto");
