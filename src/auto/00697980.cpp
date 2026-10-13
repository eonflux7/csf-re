#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(char, __cdecl, CoreFiles_ContainsMatchingValue, 0x00697920, (int query, void* entries));
CSF_FUNC(char, __fastcall, CoreFiles_MatchesEntryValue, 0x0069E7B0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __cdecl CoreFiles_CloseSceneFile(int file)
{
    void* options = g_pOptions;
    void* specialEntries = CSF_AT(void*, options, 0x60);
    if (specialEntries)
    {
        if (CoreFiles_MatchesEntryValue(specialEntries, nullptr, file))
            return;
        options = g_pOptions;
    }

    const uint32_t begin = CSF_AT(uint32_t, options, 0x54);
    if (begin)
    {
        const uint32_t end = CSF_AT(uint32_t, options, 0x58);
        if ((static_cast<int32_t>(end - begin) >> 5) != 0)
        {
            if (CoreFiles_ContainsMatchingValue(
                    file, static_cast<uint8_t*>(options) + 0x50))
                return;
            options = g_pOptions;
        }
    }

    const uint32_t count = CSF_AT(uint32_t, options, 0x28);
    if (!count)
        return;

    int* entries = CSF_AT(int*, options, 0x20);
    uint32_t index = 0;
    while (entries[index] != file)
    {
        if (++index >= count)
            return;
    }

    void* object = reinterpret_cast<void*>(entries[index]);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);

        options = g_pOptions;
        CSF_AT(int*, options, 0x20)[index] = 0;
    }

    const uint32_t remaining = CSF_AT(uint32_t, options, 0x28) - 1;
    CSF_AT(uint32_t, options, 0x28) = remaining;
    if (index != remaining)
    {
        int* currentEntries = CSF_AT(int*, options, 0x20);
        _memcpy(currentEntries + index, currentEntries + index + 1,
                4 * (remaining - index));
    }
}

HOOK(0x00697980, CoreFiles_CloseSceneFile, "auto");
