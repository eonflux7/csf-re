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
CSF_FUNC(int, __fastcall, AssignTableWordVector, 0x0051A020, (S_0046c410* self, void* /*edx*/, void* source));

// Copies the records in [sourceBegin, sourceEnd) into consecutive 44-byte
// slots starting at destinationBegin. Returns the destination slot one past
// the last copied record, or destinationBegin when the range is empty.
static int __cdecl CopyGameDataRecordRange(int sourceBegin, int sourceEnd, int destinationBegin)
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

        AssignTableWordVector(
            reinterpret_cast<S_0046c410*>(static_cast<std::uintptr_t>(destinationRecord + 0x1C)),
            nullptr,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(sourceRecord + 0x1C)));

        sourceRecord += 0x2C;
        destinationRecord += 0x2C;
    } while (sourceRecord != sourceEnd);

    return destinationRecord;
}

HOOK(0x776890, CopyGameDataRecordRange, "auto");
