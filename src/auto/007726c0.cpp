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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_772500, 0x00772500, (void* self, void* edx, int a5));

// Copies the animation records in [sourceBegin, sourceEnd) into consecutive
// 44-byte slots starting at destinationBegin. Returns the destination slot one
// past the last copied record, or destinationBegin when the range is empty.
static int __cdecl AnimationDefs_CopyRecordRange(int sourceBegin, int sourceEnd, int destinationBegin)
{
    if (sourceBegin == sourceEnd) {
        return destinationBegin;
    }

    int sourceRecord = sourceBegin;
    int destinationRecord = destinationBegin;
    do {
        AssignStringSubstring(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(destinationRecord)),
            nullptr,
            reinterpret_cast<const void*>(static_cast<std::uintptr_t>(sourceRecord)),
            0,
            0xFFFFFFFFu);

        sub_772500(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(destinationRecord + 0x1C)),
            nullptr,
            sourceRecord + 0x1C);

        sourceRecord += 0x2C;
        destinationRecord += 0x2C;
    } while (sourceRecord != sourceEnd);

    return destinationRecord;
}

HOOK(0x7726C0, AnimationDefs_CopyRecordRange, "auto");
