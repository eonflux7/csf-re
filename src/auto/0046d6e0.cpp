#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 32-byte (0x20) record; the original passes its 4th argument in ecx.
CSF_FUNC(std::uint32_t, __stdcall, Record_ApplyAction_697AD0, 0x00697AD0,
         (C_0082d410* a1, C_0082d410* a2));

// Destroys one already-constructed record.
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270,
         (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 32-byte (0x20) record.
static C_0082d410* MissionScript_NextRecord(C_0082d410* record)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(record) + 0x20);
}

// Applies Record_ApplyAction_697AD0 to each of `count` consecutive records starting at
// `first`, passing `actionArg` as its second argument. If an action throws, the records
// already processed are destroyed again and the exception is rethrown.
static int __cdecl MissionScript_ApplyToRecordRange(
    C_0082d410* first,
    std::uint32_t count,
    C_0082d410* actionArg,
    C_0082d410* /*context*/)
{
    C_0082d410* currentRecord = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(Record_ApplyAction_697AD0(currentRecord, actionArg));
            --count;
            currentRecord = MissionScript_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        C_0082d410* record = first;
        while (record != currentRecord)
        {
            DestroyRecord_469270(static_cast<void*>(record));
            record = MissionScript_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046D6E0, MissionScript_ApplyToRecordRange, "auto");
