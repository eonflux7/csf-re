#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Assigns a source value into one 32-byte (0x20) record.
CSF_FUNC(std::uint32_t, __stdcall, CoreFiles_AssignEntryToSlot, 0x00697AD0,
         (C_0082d410* destination, C_0082d410* source));

// Releases one already-assigned record (MissionScript record cleanup).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 32-byte (0x20) record.
static C_0082d410* CoreFiles_NextEntry(C_0082d410* entry)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(entry) + 0x20);
}

// Assigns `source` to each of `count` consecutive 32-byte records starting at `first`.
// If an assignment throws, the records already processed are released again and the
// exception is rethrown.
static int __cdecl CoreFiles_AssignEntriesToSlots(
    C_0082d410* first,
    std::uint32_t count,
    void* source,
    std::uint32_t /*unused*/)
{
    C_0082d410* currentEntry = first;
    std::uint32_t result = 0;

    try
    {
        while (count != 0)
        {
            result = CoreFiles_AssignEntryToSlot(currentEntry,
                                                 reinterpret_cast<C_0082d410*>(source));
            --count;
            currentEntry = CoreFiles_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        C_0082d410* entry = first;
        while (entry != currentEntry)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(entry));
            entry = CoreFiles_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(result);
}

HOOK(0x00697C50, CoreFiles_AssignEntriesToSlots, "auto");
