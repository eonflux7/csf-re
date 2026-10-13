#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a 4-byte header and an embedded record body, resets the entry's
// length/capacity fields, and assigns the trailing name string.
CSF_FUNC(int, __fastcall, SoundDefs_CopyRecordBody, 0x0075A6C0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static int __stdcall SoundDefs_CopyEntry(int entry, std::uint32_t* source)
{
    int result = 0;

    if (entry) {
        std::uint8_t* record = reinterpret_cast<std::uint8_t*>(entry);

        CSF_AT(std::uint32_t, record, 0) = source[0];
        SoundDefs_CopyRecordBody(record + 4, nullptr,
                                 static_cast<int>(reinterpret_cast<std::uintptr_t>(source + 1)));

        CSF_AT(int, record, 64) = 15;
        CSF_AT(int, record, 60) = 0;
        CSF_AT(std::uint8_t, record, 44) = 0;

        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
            AssignStringSubstring(record + 40, nullptr, source + 10, 0, 0xFFFFFFFFu)));
    }

    return result;
}

HOOK(0x75AEF0, SoundDefs_CopyEntry, "auto");
