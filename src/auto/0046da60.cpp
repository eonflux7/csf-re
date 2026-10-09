#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs a run of 8-byte elements into raw storage.
CSF_FUNC(int, __cdecl, ConstructElementCopies8_46C020,
         0x0046C020,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies of an 8-byte element.
CSF_FUNC(unsigned int, __fastcall, InsertElementCopies8_46D3E0,
         0x0046D3E0,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

static int __fastcall MissionScript_AppendElement8(
    void* self, void* /*edx*/, int* source)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 3)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 3))
    {
        return static_cast<int>(InsertElementCopies8_46D3E0(
            self, nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, self, 8)),
            1,
            reinterpret_cast<uint32_t*>(source)));
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 8);

    const int result = ConstructElementCopies8_46C020(
        static_cast<int>(insertionAddress), 1,
        reinterpret_cast<int>(source),
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(source));

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 8;
    return result;
}

HOOK(0x0046DA60, MissionScript_AppendElement8, "auto");
