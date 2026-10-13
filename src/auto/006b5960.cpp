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

// Processes one 16-byte (0x10) record with a forwarded argument.
CSF_FUNC(int*, __stdcall, ProcessRecord16WithArg, 0x006B5510, (void* a1, int a2));

// Destroys one already-constructed 16-byte (0x10) record.
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220, (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 16-byte (0x10) record.
static std::uint8_t* Records16_NextEntry(std::uint8_t* entry)
{
    return entry + 0x10;
}

// Forwards `forwardArg` to each of `count` consecutive 16-byte records starting at
// `first`. If a forward call throws, the records already processed are destroyed
// again and the exception is rethrown.
static int* __cdecl Records16_ForwardToEntries(
    std::uint8_t* first,
    std::uint32_t count,
    int forwardArg,
    void* contextObject,
    [[maybe_unused]] int unusedArg)
{
    std::uint8_t* currentEntry = first;
    int* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = ProcessRecord16WithArg(currentEntry, forwardArg);
            --count;
            currentEntry = Records16_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            DestroyElementThroughContext(reinterpret_cast<int*>(entry));
            entry = Records16_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006B5960, Records16_ForwardToEntries, "auto");
