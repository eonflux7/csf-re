#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SoundManager_ProcessInstanceEntry(void* self, void* /*edx*/, void* entry)
{
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, entry, 0x1C);

    bool inactive = false;
    if (CSF_AT(std::uint8_t, entry, 0) == 0)
    {
        void* object = CSF_AT(void*, entry, 4);
        inactive = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, object, 0)[0x80 / 4])(object, nullptr, 0, entry);
    }

    CSF_AT(std::uint8_t, entry, 0) = inactive;
    CSF_AT(std::uint32_t, entry, 0x1C) = 0;

    if (!inactive)
    {
        void* object = CSF_AT(void*, entry, 4);
        CSF_AT(std::uint8_t, entry, 0x15) = 0;
        CSF_AT(std::uint8_t, entry, 0x16) = 0;
        CSF_AT(std::uint8_t, entry, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, object, 0)[0x38 / 4])(object, nullptr, self, entry) == -1;
    }

    const bool needsFallback = CSF_AT(std::uint8_t, entry, 0) == 0;
    CSF_AT(std::uint32_t, entry, 0x1C) = 0;
    if (needsFallback)
    {
        void* object = CSF_AT(void*, entry, 4);
        CSF_AT(std::uint8_t, entry, 0x15) = 0;
        CSF_AT(std::uint8_t, entry, 0x16) = 0;
        CSF_AT(std::uint8_t, entry, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, object, 0)[0x54 / 4])(
                    object, nullptr, static_cast<std::uint8_t*>(self) + 0xC, entry) == -1;
    }

    CSF_AT(std::uint32_t, entry, 0x1C) = savedContext;

    if (CSF_AT(std::uint8_t, entry, 0) != 0)
    {
        CSF_AT(std::uint8_t, entry, 0) = 1;
        return 1;
    }

    void* object = CSF_AT(void*, entry, 4);
    const int result = reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr, entry);

    if (static_cast<std::uint8_t>(result) == 0)
    {
        CSF_AT(std::uint8_t, entry, 0) = 1;
        return 1;
    }

    CSF_AT(std::uint8_t, entry, 0) = 0;
    return result;
}

HOOK(0x00523C40, SoundManager_ProcessInstanceEntry, "auto");
