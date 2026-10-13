#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee: 0x0078BDA0, returns char*, result unused by the caller.
CSF_FUNC(char*, __fastcall, CopyCounterDescriptor, 0x0078BDA0, (void* self, void* edx, int a2));

// Callee: 0x004109E0, assigns a substring into the string member.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Fills every 164-byte perf-counter record in [firstRecord, endRecord) from one source record.
// The source record holds a dword id at +0, a descriptor at +4 (copied into record+4) and a
// string at +0x88 (assigned into record+0x88, i.e. record+136, as the whole string).
static int __cdecl NetPerfCounters_FillRecordRange(int firstRecord, int endRecord, std::uint32_t* sourceRecord)
{
    int result = endRecord;
    int current = firstRecord;
    while (current != endRecord)
    {
        *reinterpret_cast<std::uint32_t*>(current) = *sourceRecord;
        CopyCounterDescriptor(reinterpret_cast<void*>(current + 4), nullptr,
                              reinterpret_cast<int>(sourceRecord + 1));
        AssignStringSubstring(reinterpret_cast<void*>(current + 136), nullptr,
                              reinterpret_cast<const void*>(sourceRecord + 34), 0u, 0xFFFFFFFFu);
        result = endRecord;
        current += 164;
    }
    return result;
}

HOOK(0x0078C2F0, NetPerfCounters_FillRecordRange, "auto");
