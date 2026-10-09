#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Inserts a single 24-byte element at `position` (calls the real insertion
// routine at 0x454590; not analysed here - marked "todo").
CSF_FUNC(int, __fastcall, InsertElementArray24At, 0x00454590,
         (void* self, void* edx, char* a1, unsigned int a3, std::uint32_t* arg8));

// Computes the index of `position` inside the 24-byte element array owned by
// `self`, inserts one element there, and writes the address of the (possibly
// reallocated) inserted element into `result`, which is also returned.
static std::uint32_t* __fastcall InsertElement24(
    S_00454c80* self, void* /*edx*/, std::uint32_t* result,
    char* position, std::uint32_t* element)
{
    std::int32_t index = 0;
    const std::uint32_t begin = self->field_0004;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(
            self->field_0008 - begin);

        if (sizeBytes / 24 != 0)
        {
            const std::uint32_t positionAddress =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(position));
            index = static_cast<std::int32_t>(positionAddress - begin) / 24;
        }
    }

    InsertElementArray24At(self, nullptr, position, 1u, element);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 24u;
    return result;
}

HOOK(0x00454C80, InsertElement24, "auto");
