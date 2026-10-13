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

// Processes one 44-byte (0x2C) file-system entry. Called with the context object in ecx
// and two stack arguments (entry, forward argument); the callee pops 8 bytes.
CSF_FUNC(uint32_t, __fastcall, RtFS_ProcessFileSystemEntry, 0x0069EA90,
         (C_0082d410* self, void* edx, C_0082d410* entry, C_0082d410* forwardArg));

// Releases one entry's buffer during unwinding (stdcall, one argument, pops 4).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 44-byte (0x2C) entry.
static std::uint8_t* RtFS_NextEntry(std::uint8_t* entry)
{
    return entry + 0x2C;
}

// Applies `forwardArg` to each of `count` consecutive 44-byte entries starting at `first`,
// using `contextObject` as the receiver. If a call throws, the entries already processed
// are released again and the exception is rethrown.
static int __cdecl RtFS_ForwardToFileSystemEntries(
    std::uint8_t* first,
    std::uint32_t count,
    uint32_t* forwardArg,
    C_0082d410* contextObject,
    void* /*unusedArg*/)
{
    std::uint8_t* currentEntry = first;
    uint32_t result = 0;

    try
    {
        while (count != 0)
        {
            result = RtFS_ProcessFileSystemEntry(
                contextObject, nullptr,
                reinterpret_cast<C_0082d410*>(currentEntry),
                reinterpret_cast<C_0082d410*>(forwardArg));
            --count;
            currentEntry = RtFS_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(entry));
            entry = RtFS_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(result);
}

HOOK(0x0069EBC0, RtFS_ForwardToFileSystemEntries, "auto");
