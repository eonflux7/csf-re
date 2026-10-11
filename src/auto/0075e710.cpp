#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Inserts a single 72-byte waypoint record at `position` (calls the real
// insertion routine; not analysed here - marked "todo").
CSF_FUNC(char, __fastcall, sub_75E390, 0x0075E390,
         (void* self, void* edx, char* a1, unsigned int a3, int arg8));

// Inserts a waypoint record into the 72-byte-element array at `position` and
// writes the address of the inserted element (after the insertion, the array may
// have moved) into `result`, which is also returned.
static uint32_t* __fastcall InsertWaypointRecord(
    S_0075e710* self, void* /*edx*/, uint32_t* result,
    char* position, int value)
{
    const uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(self->field_0008 - begin);

        if (sizeBytes / 72 != 0)
        {
            const std::uintptr_t positionAddress =
                reinterpret_cast<std::uintptr_t>(position);
            index = static_cast<std::int32_t>(positionAddress - begin) / 72;
        }
    }

    sub_75E390(self, nullptr, position, 1u, value);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 72u;
    return result;
}

HOOK(0x0075E710, InsertWaypointRecord, "auto");
