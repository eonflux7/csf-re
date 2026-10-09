#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Inserts a single 24-byte element at `position` (calls the real insertion
// routine; not analysed here - marked "todo").
CSF_FUNC(int, __fastcall, InsertRecord24At, 0x00768800,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// Inserts an element into the 24-byte-element array at `position` and writes the
// address of the inserted element (after the insertion, the array may have moved)
// into `result`, which is also returned.
static uint32_t* __fastcall InsertElement24(
    S_00768bf0* self, void* /*edx*/, uint32_t* result,
    char* position, uint32_t* value)
{
    const uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(self->field_0008 - begin);

        if (sizeBytes / 24 != 0)
        {
            const std::uint32_t positionAddress =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(position));
            index = static_cast<std::int32_t>(positionAddress - begin) / 24;
        }
    }

    InsertRecord24At(self, nullptr, position, 1u, value);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 24u;
    return result;
}

HOOK(0x00768BF0, InsertElement24, "auto");
