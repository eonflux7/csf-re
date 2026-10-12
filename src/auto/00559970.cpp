#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Applies one argument to one 20-byte (0x14) record. Called with the owner object in ecx;
// the packet's "bind as" line lists __stdcall, but the original sets ecx = owner before the call,
// so it is bound here with the thiscall-compatible convention (self in ecx, edx unused, ret 8).
CSF_FUNC(int, __fastcall, MpRules_ApplyArgToRecord, 0x00559760,
         (void* self, void* edx, int record, std::uint32_t* arg));

// Tears down one already-constructed 0x14-byte record. Original passes the owner in ecx
// (thiscall, ret 4).
CSF_FUNC(int, __fastcall, MpRules_DestroyRecord, 0x00559830,
         (void* self, void* edx, int record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 20-byte (0x14) record.
static std::uint8_t* MpRules_NextRecord(std::uint8_t* record)
{
    return record + 0x14;
}

// Applies `arg` to each of `count` consecutive 0x14-byte records starting at `first`,
// using `owner` as the call receiver. If a call throws, the records already processed are
// destroyed again and the exception is rethrown.
static int __cdecl MpRules_ApplyToRecordArray(
    std::uint8_t* first,
    std::uint32_t count,
    std::uint32_t* arg,
    void* owner)
{
    std::uint8_t* currentRecord = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = MpRules_ApplyArgToRecord(
                owner,
                nullptr,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(currentRecord)),
                arg);
            --count;
            currentRecord = MpRules_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        std::uint8_t* record = first;
        while (record != currentRecord)
        {
            MpRules_DestroyRecord(
                owner,
                nullptr,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(record)));
            record = MpRules_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00559970, MpRules_ApplyToRecordArray, "auto");
