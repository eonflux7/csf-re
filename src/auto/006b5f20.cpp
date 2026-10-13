#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a run of 16-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int*, __cdecl, Container16_InitElementRange_6B5960,
         0x006B5960,
         (int* a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(char, __fastcall, Container16_InsertElementCopies_6B5A90,
         0x006B5A90,
         (void* self, void* edx, void* a1, unsigned int a3, int a2));

// Appends one 16-byte element to the vector-like container (__thiscall, ret 4).
static int* __fastcall Container16_AppendElement(
    void* self, void* /*edx*/, void* source)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 4)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 4))
    {
        const char insertResult = Container16_InsertElementCopies_6B5A90(
            self, nullptr,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 8))),
            1u,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));
        return reinterpret_cast<int*>(static_cast<std::intptr_t>(insertResult));
    }

    const std::uint32_t insertionAddress = CSF_AT(std::uint32_t, self, 8);

    int* const result = Container16_InitElementRange_6B5960(
        reinterpret_cast<int*>(static_cast<std::uintptr_t>(insertionAddress)),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 16;
    return result;
}

HOOK(0x006B5F20, Container16_AppendElement, "auto");

