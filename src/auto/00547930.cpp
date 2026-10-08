#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs one 68-byte element into the uninitialized slot at `destination`.
CSF_FUNC(std::uint32_t*, __cdecl, CopyConstructElement68, 0x00546E80,
         (std::uint32_t* destination, int count, const std::uint32_t* source,
          void* container, const std::uint32_t* sourceCopy));

// Inserts one 68-byte element at `position`, reallocating the buffer if needed.
CSF_FUNC(std::uint32_t*, __fastcall, InsertElement68, 0x00547690,
         (void* self, void* edx, std::uint32_t* result,
          char* position, const std::uint32_t* element));

// Inline-vector-like container of 68-byte elements.
//   +0x04 = begin pointer, +0x08 = end pointer, +0x0C = capacity pointer.
static std::uint32_t* __fastcall AppendElement68(
    void* self, void* /*edx*/, std::uint32_t* element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x08) - begin) / 68);
    }

    if (begin != 0)
    {
        const std::uint32_t capacity = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x0C) - begin) / 68);

        if (size < capacity)
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);
            std::uint32_t* const result = CopyConstructElement68(
                reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)),
                1, element, self, element);

            CSF_AT(std::uint32_t, self, 0x08) = end + 68u;
            return result;
        }
    }

    char* const position = reinterpret_cast<char*>(
        CSF_AT(std::uint32_t, self, 0x08));
    return InsertElement68(
        self, nullptr, reinterpret_cast<std::uint32_t*>(&element),
        position, element);
}

HOOK(0x00547930, AppendElement68, "auto");
