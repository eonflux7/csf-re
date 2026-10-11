#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned int, __fastcall, InsertElementsAtPosition, 0x0047EF80,
         (void* self, void* edx, int* Src, unsigned int a7, int* a4));

static std::uint32_t* __fastcall VectorInsertOneAndReturnIterator(
    int* self, void* /*edx*/, std::uint32_t* result,
    void* position, int* value)
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

    InsertElementsAtPosition(self, nullptr,
                             reinterpret_cast<int*>(position), 1u, value);

    *result = CSF_AT(std::uint32_t, self, 0x04)
            + static_cast<std::uint32_t>(index) * 4u;
    return result;
}

HOOK(0x0047EF30, VectorInsertOneAndReturnIterator, "auto");
