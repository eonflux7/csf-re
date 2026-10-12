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

// Processes one 52-byte (0x34) record with a forwarded argument.
CSF_FUNC(int, __stdcall, sub_774F50, 0x00774F50, (void* a1, int a2));

// Releases one already-constructed 52-byte (0x34) record.
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 52-byte (0x34) record.
static std::uint8_t* MissionScript_NextRecord(std::uint8_t* record)
{
    return record + 0x34;
}

// Forwards `forwardArg` to each of `count` consecutive 52-byte records starting at
// `first`. If a forward call throws, the records already processed are released
// again and the exception is rethrown. The fourth argument is not used by the
// original; the fifth is the object the original passes in ecx to every call.
static int __cdecl MissionScript_ForwardArgToRecords(
    std::uint8_t* first,
    std::uint32_t count,
    int forwardArg,
    int /*unusedArg*/,
    int contextObject)
{
    std::uint8_t* currentRecord = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = sub_774F50(currentRecord, forwardArg);
            --count;
            currentRecord = MissionScript_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        std::uint8_t* record = first;
        while (record != currentRecord)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(record));
            record = MissionScript_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00775250, MissionScript_ForwardArgToRecords, "auto");
