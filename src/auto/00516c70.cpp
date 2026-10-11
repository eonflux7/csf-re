#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one save-game record with a caller-supplied argument.
CSF_FUNC(int, __stdcall, SaveGameRecord_Process, 0x00516960,
         (void* a1, void* a2));

// Destructs one already-constructed save-game record (unwind path).
CSF_FUNC(int, __stdcall, SaveGameRecord_Destruct, 0x00514F30,
         (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 80-byte (0x50) save-game record.
static S_004f21d0* SaveGameRecords_NextRecord(S_004f21d0* record)
{
    return reinterpret_cast<S_004f21d0*>(reinterpret_cast<std::uint8_t*>(record) + 0x50);
}

// Processes `count` consecutive records starting at `first`, passing `arg` to each.
// If a call throws, the records already processed are destructed and the exception
// is rethrown.
static int __cdecl SaveGameRecords_ProcessRange(
    S_004f21d0* first,
    std::uint32_t count,
    void* arg,
    int /*unused*/,
    void* context)
{
    S_004f21d0* currentRecord = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            // `context` is loaded into ecx by the original; the stdcall binding
            // above does not take a this-pointer, so it is not forwarded here.
            result = SaveGameRecord_Process(
                static_cast<void*>(currentRecord),
                arg);
            --count;
            currentRecord = SaveGameRecords_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        S_004f21d0* record = first;
        while (record != currentRecord)
        {
            SaveGameRecord_Destruct(reinterpret_cast<int*>(record));
            record = SaveGameRecords_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00516C70, SaveGameRecords_ProcessRange, "auto");
