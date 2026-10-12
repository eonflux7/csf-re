#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Inserts a single 20-byte element at `position` (calls the real insertion
// routine; not analysed here - marked "todo").
CSF_FUNC(void*, __fastcall, MpRules_InsertRecord20At, 0x005590F0,
         (void* self, void* edx, int* a1, unsigned int a3, int* arg8));

// Inserts an element into the 20-byte-element array at `position` and writes the
// address of the inserted element (after the insertion, the array may have moved)
// into `result`, which is also returned.
static uint32_t* __fastcall MpRules_InsertElement20AndGetAddress(
    S_005594b0* self, void* /*edx*/, uint32_t* result,
    int* position, int* value)
{
    const uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(self->field_0008 - begin);

        if (sizeBytes / 20 != 0)
        {
            const std::uint32_t positionAddress =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(position));
            index = static_cast<std::int32_t>(positionAddress - begin) / 20;
        }
    }

    MpRules_InsertRecord20At(self, nullptr, position, 1u, value);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 20u;
    return result;
}

HOOK(0x005594B0, MpRules_InsertElement20AndGetAddress, "auto");
