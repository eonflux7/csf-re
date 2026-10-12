#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a run of 32-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, Minimap_InitElementRange32_43D330,
         0x0043D330,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(unsigned int, __fastcall, Minimap_InsertElementCopies32_43D890,
         0x0043D890,
         (void* self, void* edx, int a1, unsigned int a3, void* arg8));

// Appends one 32-byte element to the vector-like container (__thiscall, ret 4).
static int __fastcall Minimap_AppendElement32(
    void* self, void* /*edx*/, void* source)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 5)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 5))
    {
        const unsigned int insertResult = Minimap_InsertElementCopies32_43D890(
            self, nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, self, 8)),
            1,
            source);
        return static_cast<int>(insertResult);
    }

    const std::uint32_t insertionAddress = CSF_AT(std::uint32_t, self, 8);

    const int result = Minimap_InitElementRange32_43D330(
        static_cast<int>(insertionAddress),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 32;
    return result;
}

HOOK(0x0043DAF0, Minimap_AppendElement32, "auto");
