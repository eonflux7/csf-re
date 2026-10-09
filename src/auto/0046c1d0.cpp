#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases one mission-script record entry during exception cleanup.
CSF_FUNC(int, __stdcall, MissionScript_ReleaseEntry, 0x0046C090,
         (int a1));

// Copies a dword pair from src to dest when dest is non-null.
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Copies `count` consecutive 8-byte records starting at `firstDest`, each time
// from the same source pointer. If a copy throws, the records already processed
// are released again and the exception is rethrown.
static int __cdecl MissionScript_CopyRecordPairs(
    int firstDest,
    int count,
    int srcPtr,
    int /*ctx*/,
    int /*unused*/)
{
    int currentDest = firstDest;
    int result = 0;

    try
    {
        while (count != 0)
        {
            uint32_t* copied = CopyTwoDwordsIfDestNonNull(
                reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(currentDest)),
                reinterpret_cast<const uint32_t*>(static_cast<std::uintptr_t>(srcPtr)));
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(copied));
            --count;
            currentDest += 8;
        }
    }
    catch (...)
    {
        int entry = firstDest;
        while (entry != currentDest)
        {
            MissionScript_ReleaseEntry(entry);
            entry += 8;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046C1D0, MissionScript_CopyRecordPairs, "auto");
