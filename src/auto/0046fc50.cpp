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

// Copy-constructs one 28-byte (0x1C) script record from `source` into `destination`.
// The original also loads its 4th argument into ecx before this call; that register
// value is not representable with __stdcall and is noted in the metadata.
CSF_FUNC(int*, __stdcall, CopyConstructRecord_410B50, 0x00410B50,
         (void* destination, const void* source));

// Destroys one already-constructed 28-byte script record.
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270,
         (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 28-byte (0x1C) script record.
static C_0082d410* MissionScript_NextRecord(C_0082d410* record)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(record) + 0x1C);
}

// Copy-constructs `count` consecutive records starting at `first` from `source`.
// If a copy throws, the records already constructed are destroyed again and the
// exception is rethrown. Returns the result of the last copy construction (eax).
static int* __cdecl MissionScript_CopyConstructRecords(
    C_0082d410* first,
    std::uint32_t count,
    const void* source,
    void* /*context*/,
    int /*unused*/)
{
    C_0082d410* currentRecord = first;
    int* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyConstructRecord_410B50(
                static_cast<void*>(currentRecord),
                source);
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

HOOK(0x0046FC50, MissionScript_CopyConstructRecords, "auto");
