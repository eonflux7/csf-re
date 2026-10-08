#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, InsertElementArray36, 0x007278A0,
         (void* self, void* edx, char* position,
          unsigned int count, int* element));

static std::uint32_t* __fastcall InsertElement36(
    void* self, void* /*edx*/, std::uint32_t* result,
    char* position, int* element)
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

    InsertElementArray36(self, nullptr, position, 1u, element);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 36u;
    return result;
}

HOOK(0x00727F90, InsertElement36, "auto");
