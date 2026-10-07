#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall UpdateEntriesFromMatchingId(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x40);
    const int selectedId =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[90])(
            owner, nullptr);

    CSF_AT(std::uint32_t, self, 0x6C) = 0;

    void** begin = CSF_AT(void**, self, 0x48);
    int result = static_cast<int>(reinterpret_cast<std::uintptr_t>(begin));
    std::uint32_t count = 0;

    if (begin != nullptr)
    {
        const std::uint32_t byteDifference =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(
                CSF_AT(void**, self, 0x4C))) -
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(begin));

        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    for (std::uint32_t index = 0; index < count; ++index)
    {
        void* entry = CSF_AT(void**, self, 0x48)[index];
        result =
            ((int(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[8])(
                entry, nullptr);

        if (result == selectedId)
        {
            CSF_AT(std::uint32_t, self, 0x6C) = index;
            break;
        }
    }

    for (std::uint32_t index = 0; index < count; ++index)
    {
        if (index >= CSF_AT(std::uint32_t, self, 0x6C))
        {
            void* entry = CSF_AT(void**, self, 0x48)[index];
            result =
                ((int(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[5])(
                    entry, nullptr);
        }
    }

    return result;
}

HOOK(0x0071AA00, UpdateEntriesFromMatchingId, "auto");
