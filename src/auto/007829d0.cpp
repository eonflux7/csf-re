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

// Processes one 0x15C-byte record with a forwarded argument; `this` is passed in ecx.
CSF_FUNC(int, __stdcall, ForwardToMissionRecord, 0x00782910, (void* a1, void* a2));

// Releases the resources of one already-processed 0x15C-byte record; `this` is passed in ecx.
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 0x15C-byte (348-byte) record.
static std::uint8_t* MissionRecord_NextEntry(std::uint8_t* entry)
{
    return entry + 0x15C;
}

// Forwards `forwardArg` to each of `count` consecutive 0x15C-byte records starting at
// `first`, with `contextObject` as the call context. If a forward call throws, the records
// already processed are released again and the exception is rethrown.
static int __cdecl MissionScript_ForwardToRecordBlock(
    std::uint8_t* first,
    std::uint32_t count,
    void* forwardArg,
    void* contextObject)
{
    std::uint8_t* currentEntry = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = ForwardToMissionRecord(currentEntry, forwardArg);
            --count;
            currentEntry = MissionRecord_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(entry));
            entry = MissionRecord_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x007829D0, MissionScript_ForwardToRecordBlock, "auto");
