#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_514B10, 0x00514B10, (void* self, void* edx, int a4a));

static S_004f21d0* __cdecl SaveGameEntries_CopyRangeFromSource(S_004f21d0* first, S_004f21d0* last, std::uint32_t* source)
{
    S_004f21d0* result = last;
    for (S_004f21d0* entry = first; entry != last;
         entry = reinterpret_cast<S_004f21d0*>(reinterpret_cast<std::uint8_t*>(entry) + 0x50))
    {
        AssignStringRange_488830(entry, nullptr, source, 0, 0xFFFFFFFF);

        entry->field_001c = source[7];
        entry->field_0020 = source[8];
        entry->field_0024 = source[9];
        entry->field_0028 = source[10];
        entry->field_002c = source[11];
        entry->field_0030 = source[12];

        sub_514B10(reinterpret_cast<void*>(reinterpret_cast<std::uint8_t*>(entry) + 0x34), nullptr,
                   static_cast<int>(reinterpret_cast<std::uintptr_t>(source + 13)));

        CSF_AT(std::uint8_t, entry, 0x44) = CSF_AT(std::uint8_t, source, 0x44);
        entry->field_0048 = source[18];
        result = last;
        CSF_AT(std::uint8_t, entry, 0x4c) = CSF_AT(std::uint8_t, source, 0x4c);
        CSF_AT(std::uint8_t, entry, 0x4d) = CSF_AT(std::uint8_t, source, 0x4d);
    }
    return result;
}

HOOK(0x00516850, SaveGameEntries_CopyRangeFromSource, "auto");
