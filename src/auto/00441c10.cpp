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

// Processes one 16-byte (0x10) entry with a forwarded argument. Callee is stdcall (ret 8);
// the original also loads a4 into ecx before the call, which a stdcall callee ignores.
CSF_FUNC(int, __stdcall, MissionScript_ForwardEntryIfNonNull, 0x004685B0, (int a1, int a2));

// Frees the handle stored in one already-processed 16-byte (0x10) entry.
CSF_FUNC(int, __stdcall, MissionScript_FreeAndClearHandle, 0x00468940, (int* handle));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 16-byte (0x10) entry.
static std::uint8_t* DynamicObj_NextEntry(std::uint8_t* entry)
{
    return entry + 0x10;
}

// Forwards `forwardArg` to each of `count` consecutive 16-byte entries starting at `first`.
// If a forward call throws, the entries already processed are cleaned up and the exception
// is rethrown.
static int __cdecl DynamicObj_ForwardArgToEntries(
    std::uint8_t* first,
    std::uint32_t count,
    int forwardArg)
{
    std::uint8_t* currentEntry = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = MissionScript_ForwardEntryIfNonNull(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(currentEntry)),
                forwardArg);
            --count;
            currentEntry = DynamicObj_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            MissionScript_FreeAndClearHandle(reinterpret_cast<int*>(entry));
            entry = DynamicObj_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00441C10, DynamicObj_ForwardArgToEntries, "auto");
