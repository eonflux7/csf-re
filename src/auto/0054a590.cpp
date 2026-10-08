#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, MenuVideoOptions_InsertElementArray24, 0x0054A2B0,
         (void* self, void* edx, char* position,
          unsigned int count, int* element));

// Inserts one 24-byte element at `position` into the array owned by `self`
// (begin at +0x04, end at +0x08) and writes the address of the inserted
// element into *result.
static std::uint32_t* __fastcall MenuVideoOptions_InsertElement24(
    void* self, void* /*edx*/, std::uint32_t* result,
    char* position, int* element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::int32_t index = 0;

    if (begin != 0)
    {
        const std::int32_t sizeBytes = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x08) - begin);

        if (sizeBytes / 24 != 0)
        {
            const std::uint32_t positionAddress =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(position));
            index = static_cast<std::int32_t>(positionAddress - begin) / 24;
        }
    }

    MenuVideoOptions_InsertElementArray24(self, nullptr, position, 1u, element);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 24u;
    return result;
}

HOOK(0x0054A590, MenuVideoOptions_InsertElement24, "auto");
