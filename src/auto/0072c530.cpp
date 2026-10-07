#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t, __fastcall, InsertElements, 0x0072C170,
         (int* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::uint32_t* value));

static std::uint32_t* __fastcall InsertSingleElementAndReturnIterator(
    int* self, void* /*edx*/, std::uint32_t* result,
    std::uint32_t* position, std::uint32_t* value)
{
    const std::uint32_t oldBegin = CSF_AT(std::uint32_t, self, 0x04);
    std::int32_t index = 0;

    if (oldBegin != 0)
    {
        const std::uint32_t byteSize =
            CSF_AT(std::uint32_t, self, 0x08) - oldBegin;
        if ((static_cast<std::int32_t>(byteSize) >> 2) != 0)
        {
            const std::uint32_t byteOffset =
                reinterpret_cast<std::uint32_t>(position) - oldBegin;
            index = static_cast<std::int32_t>(byteOffset) >> 2;
        }
    }

    InsertElements(self, nullptr, position, 1u, value);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 4u;
    return result;
}

HOOK(0x0072C530, InsertSingleElementAndReturnIterator, "auto");
