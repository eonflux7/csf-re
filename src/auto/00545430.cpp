#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Inserts an element into the array beginning at the given position
// (calls the real insertion routine; not analysed here - marked "todo").
CSF_FUNC(char, __fastcall, InsertElementArray36At, 0x00545100,
         (void* self, void* edx, char* position, unsigned int count, int value));

// Inserts a single 36-byte element at `position` and writes the address of the
// (possibly reallocated) inserted element into `result`, which is also returned.
static std::uint32_t* __fastcall InsertElement36(
    void* self, void* /*edx*/, std::uint32_t* result,
    char* position, int element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x08) - begin);

        if (sizeBytes / 36 != 0)
        {
            const std::uint32_t positionAddress =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(position));
            index = static_cast<std::int32_t>(positionAddress - begin) / 36;
        }
    }

    InsertElementArray36At(self, nullptr, position, 1u, element);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 36u;
    return result;
}

HOOK(0x00545430, InsertElement36, "auto");
