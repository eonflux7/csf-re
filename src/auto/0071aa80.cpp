#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall InvokeTrailingEntryCallbacks(void* self, void* /*edx*/)
{
    int result = static_cast<int>(CSF_AT(std::uint32_t, self, 0x48));
    std::uint32_t count = 0;

    if (result != 0)
    {
        const std::uint32_t byteDifference =
            CSF_AT(std::uint32_t, self, 0x4C) -
            static_cast<std::uint32_t>(result);

        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    for (std::uint32_t index = 0; index < count; ++index)
    {
        if (index >= CSF_AT(std::uint32_t, self, 0x6C))
        {
            void** entries = reinterpret_cast<void**>(
                CSF_AT(std::uint32_t, self, 0x48));
            void* entry = entries[index];

            result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, entry, 0)[6])(entry, nullptr);
        }
    }

    return result;
}

HOOK(0x0071AA80, InvokeTrailingEntryCallbacks, "auto");
