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

// Processes one 32-byte (0x20) record with a forwarded argument.
CSF_FUNC(void*, __stdcall, sub_69B930, 0x0069B930, (int a1, int a2));

// Destroys one already-constructed 32-byte (0x20) record.
CSF_FUNC(int, __stdcall, DestroyRecord, 0x0069B820, (std::uint32_t* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 32-byte (0x20) record.
static std::uint8_t* Records32_NextEntry(std::uint8_t* entry)
{
    return entry + 0x20;
}

// Forwards `forwardArg` to each of `count` consecutive 32-byte records starting at
// `first`. If a forward call throws, the records already processed are destroyed
// again and the exception is rethrown.
static void* __cdecl Records32_ForwardToEntries(
    std::uint8_t* first,
    std::uint32_t count,
    uint32_t* forwardArg,
    void* contextObject)
{
    std::uint8_t* currentEntry = first;
    void* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = sub_69B930(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(currentEntry)),
                static_cast<int>(reinterpret_cast<std::uintptr_t>(forwardArg)));
            --count;
            currentEntry = Records32_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            DestroyRecord(reinterpret_cast<std::uint32_t*>(entry));
            entry = Records32_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0069BA80, Records32_ForwardToEntries, "auto");
