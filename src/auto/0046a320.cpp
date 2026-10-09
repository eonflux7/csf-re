#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 68-byte (0x44) mission-script record. Returns a char* result.
CSF_FUNC(char*, __stdcall, sub_46A0E0, 0x0046A0E0, (char* a1, int a2));

// Destroys one already-processed 68-byte record during unwinding.
CSF_FUNC(int, __stdcall, sub_469230, 0x00469230, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 68-byte (0x44) mission-script record.
static char* MissionScript_NextRecord(char* record)
{
    return record + 0x44;
}

// Runs `count` consecutive records starting at `first`, passing `forwardArg` to each.
// If a record call throws, the records already processed are destroyed again and
// the exception is rethrown.
static char* __cdecl MissionScript_RunRecords(
    char* first,
    int count,
    int forwardArg,
    int /*unused4*/,
    int /*context*/)
{
    char* currentRecord = first;
    char* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = sub_46A0E0(currentRecord, forwardArg);
            --count;
            currentRecord = MissionScript_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        char* record = first;
        while (record != currentRecord)
        {
            sub_469230(reinterpret_cast<int*>(record));
            record = MissionScript_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046A320, MissionScript_RunRecords, "auto");
