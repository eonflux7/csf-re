#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __stdcall DestroyElementArray(void* owner)
{
    auto* elements = CSF_AT(std::uint8_t*, owner, 0x4);
    if (!elements)
        return 0;

    auto* allocation = elements - 4;
    const std::uint32_t count = CSF_AT(std::uint32_t, allocation, 0);
    auto* end = reinterpret_cast<std::uint8_t*>(
        reinterpret_cast<std::uintptr_t>(elements) + count * 0x24u);

    if (static_cast<std::int32_t>(count - 1u) >= 0)
    {
        std::uint32_t remaining = count;
        do
        {
            end -= 0x24;
            --remaining;
            CSF_AT(void**, end, 0x10) = CSF_VTBL(0x0082D410);
            CSF_AT(void**, end, 0) = CSF_VTBL(0x0082D410);
        } while (remaining);
    }

    const int result = Mem_Free(allocation);
    CSF_AT(std::uint32_t, owner, 0x4) = 0;
    return result;
}

HOOK(0x00448D50, DestroyElementArray, "auto");
