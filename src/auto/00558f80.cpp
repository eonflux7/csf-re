#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Applies the record operation at 0x558E20 to one 20-byte (0x14) record.
CSF_FUNC(int*, __stdcall, MpRules_ApplyToRecord, 0x00558E20,
         (int a1, std::uint32_t* a2));

// Tears down one already-processed 20-byte record during rollback.
CSF_FUNC(int, __stdcall, MpRules_DestroyRecord, 0x00558CF0,
         (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Unwind/exception-state data referenced by the original's EH funclet.
CSF_GLOBAL(int, g_MpRulesUnwindState, 0x0088C540);

// Advances a pointer by one 20-byte (0x14) record.
static int* MpRules_NextRecord(int* record)
{
    return reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(record) + 0x14);
}

// Applies `arg` to each of `count` consecutive 20-byte records starting at `records`.
// If an application throws, the records already processed are destroyed again
// and the exception is rethrown.
static int* __cdecl MpRules_ProcessRecordsWithRollback(
    int* records,
    std::uint32_t count,
    int* arg,
    int /*unusedA4*/,
    int /*unusedA5*/)
{
    int* currentRecord = records;
    int* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = MpRules_ApplyToRecord(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(currentRecord)),
                reinterpret_cast<std::uint32_t*>(arg));
            --count;
            currentRecord = MpRules_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        int* record = records;
        while (record != currentRecord)
        {
            MpRules_DestroyRecord(record);
            record = MpRules_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00558F80, MpRules_ProcessRecordsWithRollback, "auto");
