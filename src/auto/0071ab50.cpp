#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void __fastcall ProcessUnflaggedTailObjects(void* self, void* /*edx*/)
{
    const std::uint32_t beginAddress = CSF_AT(std::uint32_t, self, 0x48);
    const std::uint32_t count = beginAddress
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x4C) - beginAddress) >> 2)
        : 0;

    for (std::uint32_t index = 0; index < count; ++index)
    {
        if (index < CSF_AT(std::uint32_t, self, 0x6C))
            continue;

        void* entry = CSF_AT(void**, self, 0x48)[index];
        void* object =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, entry, 0)[0x20 / 4])(entry, nullptr);

        if (object)
        {
            const std::uint8_t flag =
                reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0xAC / 4])(object, nullptr);

            if (!flag)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x134 / 4])(object, nullptr);
            }
        }
    }
}

HOOK(0x0071AB50, ProcessUnflaggedTailObjects, "auto");
