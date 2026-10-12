#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(char, __fastcall, CoreFiles_MatchesEntry, 0x0069E830,
               (void* self, void* edx, char* a2));

static char __cdecl CoreFiles_ContainsMatchingEntry(char* query, void* entries)
{
    const uint32_t begin = CSF_AT(uint32_t, entries, 0x4);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, entries, 0x8) - begin) >> 5)
        : 0;

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 32)
    {
        const uint32_t entryAddress =
            CSF_AT(uint32_t, entries, 0x4) + offset;
        void* object = CSF_AT(void*, reinterpret_cast<void*>(entryAddress), 0x1C);
        if (CoreFiles_MatchesEntry(object, nullptr, query))
            return 1;
    }
    return 0;
}

HOOK(0x006978C0, CoreFiles_ContainsMatchingEntry, "auto");
