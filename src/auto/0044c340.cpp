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

// Processes one 100-byte (0x64) record with a forwarded argument.
CSF_FUNC(uint32_t*, __stdcall, sub_44C1D0, 0x0044C1D0, (uint32_t* a1, int a2));

// Destroys one already-constructed 100-byte (0x64) record.
CSF_FUNC(int, __stdcall, sub_44BF80, 0x0044BF80, (uint32_t* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 100-byte (0x64) record.
static std::uint8_t* Record100_NextEntry(std::uint8_t* entry)
{
    return entry + 0x64;
}

// Forwards `forwardArg` to each of `count` consecutive 100-byte records starting at
// `first`. If a forward call throws, the records already processed are destroyed
// again and the exception is rethrown.
static uint32_t* __cdecl Record100_ForwardToEntries(
    std::uint8_t* first,
    std::uint32_t count,
    uint32_t* forwardArg,
    void* contextObject)
{
    std::uint8_t* currentEntry = first;
    uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = sub_44C1D0(reinterpret_cast<uint32_t*>(currentEntry),
                                reinterpret_cast<int>(forwardArg));
            --count;
            currentEntry = Record100_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            sub_44BF80(reinterpret_cast<uint32_t*>(entry));
            entry = Record100_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0044C340, Record100_ForwardToEntries, "auto");
