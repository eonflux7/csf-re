#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs one 24-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, CopyConstructElementArray24, 0x00549D70,
         (std::uint32_t* destination, int count, std::uint32_t* element,
          void* container, std::uint32_t* elementValue));

// Grows the 24-byte-element array and inserts one element at a position (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, InsertElement24, 0x0054A590,
         (void* self, void* edx, std::uint32_t** elementPtr,
          std::uint32_t* position, std::uint32_t* elementValue));

// Appends one 24-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to InsertElement24.
static std::uint32_t* __fastcall CMenuVideoOptions_AppendElement24(
    void* self, void* /*edx*/, std::uint32_t* a2)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x08) - begin) / 24);
    }

    if (begin != 0)
    {
        const std::uint32_t capacity = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x0C) - begin) / 24);

        if (size < capacity)
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);
            std::uint32_t* const result = CopyConstructElementArray24(
                reinterpret_cast<std::uint32_t*>(end), 1, a2, self, a2);

            CSF_AT(std::uint32_t, self, 0x08) = end + 24u;
            return result;
        }
    }

    std::uint32_t* const position = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint32_t, self, 0x08));
    return InsertElement24(self, nullptr, &a2, position, a2);
}

HOOK(0x0054A600, CMenuVideoOptions_AppendElement24, "auto");
