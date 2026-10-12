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

// Processes one 44-byte (0x2C) animation record with a forwarded argument.
CSF_FUNC(int, __stdcall, sub_772760, 0x00772760, (void* a1, void* a2));

// Resets the record buffers of one already-constructed 44-byte (0x2C) animation record.
CSF_FUNC(int, __stdcall, BddTables_ResetRecordBuffers, 0x0051A390, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Size of one animation record in the table.
static constexpr std::uintptr_t kAnimationRecordSize = 0x2C;

// Advances a pointer by one 44-byte (0x2C) animation record.
static std::uint8_t* AnimationDefs_NextEntry(std::uint8_t* entry)
{
    return entry + kAnimationRecordSize;
}

// Forwards `forwardArg` to each of `count` consecutive 44-byte animation records starting
// at `first`. If a forward call throws, the records already processed are reset again
// and the exception is rethrown.
static int __cdecl AnimationDefs_ForwardToEntries(
    int a1,
    int a2,
    void* a3,
    int a4,
    int /*a5*/)
{
    std::uint8_t* first = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a1));
    std::uint32_t count = static_cast<std::uint32_t>(a2);
    std::uint8_t* currentEntry = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = sub_772760(currentEntry, a3);
            --count;
            currentEntry = AnimationDefs_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            BddTables_ResetRecordBuffers(reinterpret_cast<int*>(entry));
            entry = AnimationDefs_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x007728B0, AnimationDefs_ForwardToEntries, "auto");
