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

// Forwards one argument to a 164-byte (0xA4) net perf entry; returns the entry pointer or null.
CSF_FUNC(char*, __stdcall, NetPerf_ForwardToEntryIfNonNull, 0x0078BEB0, (char* a1, uint32_t* a2));

// Resets the string member of one already-constructed 164-byte (0xA4) net perf entry.
CSF_FUNC(int, __stdcall, NetPerfCounters_ResetStringToEmpty, 0x0078BD00, (void* self));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 164-byte (0xA4) net perf entry.
static std::uint8_t* NetPerfEntries_NextEntry(std::uint8_t* entry)
{
    return entry + 0xA4;
}

// Forwards `forwardArg` to each of `count` consecutive 164-byte net perf entries starting at
// `first`. If a forward call throws, the entries already processed are reset again and the
// exception is rethrown.
static uint32_t* __cdecl NetPerf_ForwardToEntries164(
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
            result = reinterpret_cast<uint32_t*>(
                NetPerf_ForwardToEntryIfNonNull(reinterpret_cast<char*>(currentEntry),
                                                reinterpret_cast<uint32_t*>(forwardArg)));
            --count;
            currentEntry = NetPerfEntries_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            NetPerfCounters_ResetStringToEmpty(entry);
            entry = NetPerfEntries_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0078C1A0, NetPerf_ForwardToEntries164, "auto");
