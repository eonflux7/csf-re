#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies a 2-dword pair from `src` into `dest` when `dest` is non-null.
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

// Releases one 8-byte water pair entry.
CSF_FUNC(int, __stdcall, ReleaseEntry_709A70, 0x00709A70,
         (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 8-byte water pair entry (two dwords).
static uint32_t* WaterPairs_NextEntry(uint32_t* entry)
{
    return entry + 2;
}

// Copies `count` consecutive 8-byte pairs from `src` into the table starting at `first`.
// If a copy throws, the pairs already written are released and the exception is rethrown.
static int __cdecl WaterPairs_CopyEntries(int a1, int a2, int a3, int a4, int a5)
{
    uint32_t* first = reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(a1)));
    uint32_t* currentEntry = first;
    std::uint32_t remaining = static_cast<std::uint32_t>(a2);
    int result = 0;

    try
    {
        while (remaining != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyTwoDwordsIfDestNonNull(currentEntry,
                                           reinterpret_cast<const uint32_t*>(
                                               static_cast<std::uintptr_t>(static_cast<std::uint32_t>(a3))))));
            --remaining;
            currentEntry = WaterPairs_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        uint32_t* entry = first;
        while (entry != currentEntry)
        {
            ReleaseEntry_709A70(static_cast<int>(reinterpret_cast<std::uintptr_t>(entry)));
            entry = WaterPairs_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0070D000, WaterPairs_CopyEntries, "auto");
