#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs one 36-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, MenuSaveGame_ConstructElementArray, 0x00544DF0,
         (std::uint32_t* destination, int count, int element,
          void* container, int elementValue));

// Grows the 36-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, InsertElement36_545430, 0x00545430,
         (void* self, void* edx, int* element, std::uint32_t* position,
          int elementValue));

// Appends one 36-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to InsertElement36_545430.
static std::uint32_t* __fastcall CMenuSaveGame_AppendElement36(
    void* self, void* /*edx*/, int element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x08) - begin) / 36);
    }

    if (begin != 0)
    {
        const std::uint32_t capacity = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x0C) - begin) / 36);

        if (size < capacity)
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);
            std::uint32_t* const result = MenuSaveGame_ConstructElementArray(
                reinterpret_cast<std::uint32_t*>(end), 1, element,
                self, element);

            CSF_AT(std::uint32_t, self, 0x08) = end + 36u;
            return result;
        }
    }

    std::uint32_t* const position = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint32_t, self, 0x08));
    return InsertElement36_545430(self, nullptr, &element, position, element);
}

HOOK(0x005454A0, CMenuSaveGame_AppendElement36, "auto");
