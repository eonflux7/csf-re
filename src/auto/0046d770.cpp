#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copy-constructs one 28-byte (0x1C) record from `source` into `destination`.
CSF_FUNC(int*, __stdcall, CopyConstructRecord_410B50, 0x00410B50,
         (void* destination, const void* source));

// Destroys one already-constructed record.
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270,
         (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 28-byte (0x1C) record.
static C_0082d410* MissionRecords_NextRecord(C_0082d410* record)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(record) + 0x1C);
}

// Copy-constructs `count` consecutive 28-byte records starting at `first` from `source`.
// If a copy throws, the records already constructed are destroyed again and the
// exception is rethrown. Returns the result of the last copy.
static int* __cdecl MissionRecords_CopyConstructRange(
    C_0082d410* first,
    std::uint32_t count,
    C_0082d410* source,
    void* recordOwner)
{
    C_0082d410* currentRecord = first;
    int* result = nullptr;
    (void)recordOwner;

    try
    {
        while (count != 0)
        {
            result = CopyConstructRecord_410B50(
                static_cast<void*>(currentRecord),
                static_cast<const void*>(source));
            --count;
            currentRecord = MissionRecords_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        C_0082d410* record = first;
        while (record != currentRecord)
        {
            DestroyRecord_469270(static_cast<void*>(record));
            record = MissionRecords_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046D770, MissionRecords_CopyConstructRange, "auto");
