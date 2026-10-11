#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Processes one waypoint record (__stdcall, 2 stack args; ret 8).
CSF_FUNC(uint32_t*, __stdcall, sub_75C330, 0x0075C330, (uint32_t* a1, uint32_t* a2));

// Tears down one already-processed 44-byte waypoint record (__stdcall, 1 stack arg; ret 4).
CSF_FUNC(char, __stdcall, WaypointRecord_Destroy, 0x0075C460, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Size of one waypoint record in bytes.
static constexpr std::uint32_t kWaypointRecordSize = 0x2C;

// Advances a pointer by one 44-byte (0x2C) waypoint record.
static std::uint8_t* WaypointGraph_NextRecord(std::uint8_t* record)
{
    return record + kWaypointRecordSize;
}

// Processes `count` consecutive waypoint records starting at `first`, handing each
// one `forwardArg`. If processing throws, the records already handled are destroyed
// again and the exception is rethrown.
static uint32_t* __cdecl WaypointGraph_ProcessRecordRange(
    std::uint8_t* first,
    std::uint32_t count,
    uint32_t* forwardArg,
    int /*context*/)
{
    std::uint8_t* currentRecord = first;
    uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = sub_75C330(reinterpret_cast<uint32_t*>(currentRecord), forwardArg);
            --count;
            currentRecord = WaypointGraph_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        std::uint8_t* record = first;
        while (record != currentRecord)
        {
            WaypointRecord_Destroy(static_cast<int>(reinterpret_cast<std::uintptr_t>(record)));
            record = WaypointGraph_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0075D120, WaypointGraph_ProcessRecordRange, "auto");
