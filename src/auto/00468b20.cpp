#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Parses one 16-byte mission-script record. The original also loads ecx from the
// caller's fourth argument before this call; see notes.
CSF_FUNC(uint32_t*, __stdcall, MissionScript_ParseRecord, 0x004685B0,
         (uint32_t* a1, uint32_t* a2));

// Releases one already-parsed 16-byte mission-script record (exception cleanup path).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecord, 0x00468940,
         (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 16-byte (0x10) mission-script record.
static uint32_t* MissionScript_NextRecord(uint32_t* record)
{
    return reinterpret_cast<uint32_t*>(reinterpret_cast<std::uint8_t*>(record) + 0x10);
}

// Parses `count` consecutive 16-byte records starting at `first`, passing `arg` as the
// second argument to each parse call. If a parse call throws, the records already
// processed are released again and the exception is rethrown.
static uint32_t* __cdecl MissionScript_ProcessRecordRange(
    uint32_t* first,
    std::uint32_t count,
    uint32_t* arg,
    int /*thisArg*/)
{
    uint32_t* currentRecord = first;
    uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = MissionScript_ParseRecord(currentRecord, arg);
            --count;
            currentRecord = MissionScript_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        uint32_t* record = first;
        while (record != currentRecord)
        {
            MissionScript_ReleaseRecord(reinterpret_cast<int*>(record));
            record = MissionScript_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00468B20, MissionScript_ProcessRecordRange, "auto");
