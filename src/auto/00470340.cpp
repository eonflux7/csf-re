#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Inserts a single 12-byte element at `position` (calls the real insertion
// routine; not analysed here - marked "todo").
CSF_FUNC(char*, __fastcall, sub_470010, 0x00470010,
         (void* self, void* edx, void* a1, unsigned int a3, int arg8));

// Inserts an element into the 12-byte-element array at `position` and writes the
// address of the inserted element (after the insertion, the array may have moved)
// into `result`, which is also returned.
static uint32_t* __fastcall InsertRecord12AndWriteAddress(
    S_00470340* self, void* /*edx*/, uint32_t* result,
    C_008313f8* position, int value)
{
    const uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(self->field_0008 - begin);

        if (sizeBytes / 12 != 0)
        {
            const std::int32_t positionAddress =
                static_cast<std::int32_t>(
                    reinterpret_cast<std::intptr_t>(position));
            index = (positionAddress - static_cast<std::int32_t>(begin)) / 12;
        }
    }

    sub_470010(self, nullptr, position, 1u, value);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 12u;
    return result;
}

HOOK(0x00470340, InsertRecord12AndWriteAddress, "auto");
