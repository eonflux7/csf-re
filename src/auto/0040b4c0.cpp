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

// Processes one 28-byte (0x1C) record with a forwarded argument.
CSF_FUNC(uint32_t*, __stdcall, sub_40AEB0, 0x0040AEB0, (uint32_t* a1, uint32_t* a2));

// Destroys one already-constructed 28-byte (0x1C) record.
CSF_FUNC(int, __stdcall, sub_40AEE0, 0x0040AEE0, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 28-byte (0x1C) record.
static std::uint8_t* Records_NextEntry(std::uint8_t* entry)
{
    return entry + 0x1C;
}

// Forwards `forwardArg` to each of `count` consecutive 28-byte records starting at
// `first`. If a forward call throws, the records already processed are destroyed
// again and the exception is rethrown.
static uint32_t* __cdecl Records_ForwardToEntries(
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
            result = sub_40AEB0(reinterpret_cast<uint32_t*>(currentEntry), forwardArg);
            --count;
            currentEntry = Records_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            sub_40AEE0(static_cast<int>(reinterpret_cast<std::uintptr_t>(entry)));
            entry = Records_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0040B4C0, Records_ForwardToEntries, "auto");
