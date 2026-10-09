#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Fills 1 uninitialized 4-byte slot at the given address (cdecl, caller pops nothing).
CSF_FUNC(int, __cdecl, IntVector_FillUninitialized,
         0x0046BFB0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts one element at the given position.
CSF_FUNC(unsigned int, __fastcall, IntVector_GrowAndInsert,
         0x0046D180,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

static int __fastcall IntVector_Append(void* self, void* /*edx*/, int value)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 2)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 2))
    {
        return static_cast<int>(IntVector_GrowAndInsert(
            self, nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, self, 8)),
            1,
            reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(value))));
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 8);

    const int result = IntVector_FillUninitialized(
        static_cast<int>(insertionAddress), 1, value,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), value);

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 4;
    return result;
}

HOOK(0x0046D9F0, IntVector_Append, "auto");
