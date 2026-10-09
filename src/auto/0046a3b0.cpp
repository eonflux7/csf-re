#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a run of 8-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, MissionScript_InitElementRange_4688D0,
         0x004688D0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(unsigned int, __fastcall, MissionScript_InsertElementCopies8_469AA0,
         0x00469AA0,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

// Appends one 8-byte element to the vector-like container (__thiscall, ret 4).
static int __fastcall MissionScript_AppendElement8(
    void* self, void* /*edx*/, int source)
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
        return static_cast<int>(MissionScript_InsertElementCopies8_469AA0(
            self, nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, self, 8)),
            1,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(source))));
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 8);

    const int result = MissionScript_InitElementRange_4688D0(
        static_cast<int>(insertionAddress),
        1,
        source,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        source);

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 8;
    return result;
}

HOOK(0x0046A3B0, MissionScript_AppendElement8, "auto");
