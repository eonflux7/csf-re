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

// Processes one 0x2C-byte (44) record with a forwarded argument.
// The original sets ecx to the context object before this call; the bind line
// below is followed as given (see notes in the metadata).
CSF_FUNC(int*, __stdcall, sub_776930, 0x00776930, (void* a1, void* a2));

// Destroys one already-constructed record.
CSF_FUNC(int, __stdcall, BddTables_ResetRecordBuffers, 0x0051A390, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 0x2C-byte (44) record.
static std::uint8_t* BddTables_NextRecord(std::uint8_t* record)
{
    return record + 0x2C;
}

// Forwards `forwardArg` to each of `count` consecutive 0x2C-byte records starting at
// `first`. If a forward call throws, the records already processed are reset
// again and the exception is rethrown.
static int* __cdecl BddTables_ForwardToRecords(
    std::uint8_t* first,
    std::uint32_t count,
    void* forwardArg,
    void* contextObject,
    int /*unused*/)
{
    std::uint8_t* currentRecord = first;
    int* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = sub_776930(currentRecord, forwardArg);
            --count;
            currentRecord = BddTables_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        std::uint8_t* record = first;
        while (record != currentRecord)
        {
            BddTables_ResetRecordBuffers(reinterpret_cast<int*>(record));
            record = BddTables_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00776F60, BddTables_ForwardToRecords, "auto");
