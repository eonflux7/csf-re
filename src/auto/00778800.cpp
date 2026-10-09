#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, GameDataDefs_CopyRecordTail, 0x00776DB0, (void* self, void* edx, int a4a));

// Walks backward from endRecord to startRecord in 72-byte records, copying each
// record into the destination block that ends at destEnd. Returns the destination
// record that corresponds to startRecord.
static int __cdecl GameDataDefs_CopyRecordRangeBackward(int startRecord, int endRecord, int destEnd, int /*unused*/)
{
    std::uintptr_t srcRecord = static_cast<std::uintptr_t>(endRecord);
    if (startRecord == endRecord)
        return destEnd;

    std::uintptr_t dstRecord = static_cast<std::uintptr_t>(destEnd);
    do
    {
        srcRecord -= 72;
        dstRecord -= 72;

        AssignStringSubstring(reinterpret_cast<void*>(dstRecord), nullptr,
                              reinterpret_cast<const void*>(srcRecord), 0u, 0xFFFFFFFFu);
        AssignStringSubstring(reinterpret_cast<void*>(dstRecord + 28), nullptr,
                              reinterpret_cast<const void*>(srcRecord + 28), 0u, 0xFFFFFFFFu);
        GameDataDefs_CopyRecordTail(reinterpret_cast<void*>(dstRecord + 56), nullptr,
                                    static_cast<int>(srcRecord + 56));
    }
    while (static_cast<int>(srcRecord) != startRecord);

    return static_cast<int>(dstRecord);
}

HOOK(0x00778800, GameDataDefs_CopyRecordRangeBackward, "auto");
