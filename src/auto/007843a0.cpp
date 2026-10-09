#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 552-byte (0x228) weapon record; the original passes its owner in ecx.
CSF_FUNC(char*, __stdcall, WeaponDefs_ProcessRecord, 0x00784290,
         (char* a1, uint32_t* a2));

// Destroys one already-processed 552-byte weapon record.
CSF_FUNC(int, __stdcall, WeaponDefs_DestroyRecord, 0x00783620,
         (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 552-byte (0x228) weapon record.
static char* WeaponDefs_NextRecord(char* record)
{
    return record + 0x228;
}

// Runs WeaponDefs_ProcessRecord over `count` consecutive weapon records starting at
// `first`, passing `arg` to each. If a call throws, the records already processed
// are destroyed again and the exception is rethrown.
static char* __cdecl WeaponDefs_ProcessRecords(
    char* first,
    uint32_t count,
    uint32_t* arg,
    void* /*owner*/)
{
    char* currentRecord = first;
    char* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = WeaponDefs_ProcessRecord(currentRecord, arg);
            --count;
            currentRecord = WeaponDefs_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        char* record = first;
        while (record != currentRecord)
        {
            WeaponDefs_DestroyRecord(static_cast<int>(reinterpret_cast<std::uintptr_t>(record)));
            record = WeaponDefs_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x007843A0, WeaponDefs_ProcessRecords, "auto");
