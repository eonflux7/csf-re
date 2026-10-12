#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __cdecl DestroyElementArrays(int begin, int end, int /*unused3*/, int /*unused4*/)
{
    for (std::uint32_t entry = static_cast<std::uint32_t>(begin);
         entry != static_cast<std::uint32_t>(end); entry += 20)
    {
        auto* entryAddress = reinterpret_cast<void*>(entry);
        const std::uint32_t arrayAddress =
            CSF_AT(std::uint32_t, entryAddress, 0x4);
        if (arrayAddress)
        {
            auto* allocation = reinterpret_cast<void*>(arrayAddress - 4);
            const std::uint32_t count = CSF_AT(std::uint32_t, allocation, 0);
            std::uint32_t elementAddress = arrayAddress + count * 36;
            if (static_cast<std::int32_t>(count - 1) >= 0)
            {
                std::uint32_t remaining = count;
                do
                {
                    elementAddress -= 0x24;
                    --remaining;
                    auto* element = reinterpret_cast<void*>(elementAddress);
                    CSF_AT(void**, element, 0x10) = CSF_VTBL(0x0082D410);
                    CSF_AT(void**, element, 0) = CSF_VTBL(0x0082D410);
                } while (remaining);
            }
            Mem_Free(allocation);
            CSF_AT(std::uint32_t, entryAddress, 0x4) = 0;
        }
    }
    return end;
}

HOOK(0x004490D0, DestroyElementArrays, "auto");
