#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a run of 32-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, MissionScript_InitElementRange32_46D6E0,
         0x0046D6E0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(unsigned int, __fastcall, MissionScript_InsertElementCopies32_46DB30,
         0x0046DB30,
         (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

// Appends one 32-byte element to the vector-like container (__thiscall, ret 4).
static int __fastcall MissionScript_AppendElement32(
    void* self, void* /*edx*/, int source)
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
        return static_cast<int>(MissionScript_InsertElementCopies32_46DB30(
            self, nullptr,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 8))),
            1,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(source))));
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 8);

    const int result = MissionScript_InitElementRange32_46D6E0(
        static_cast<int>(insertionAddress),
        1,
        source,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        source);

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 32;
    return result;
}

HOOK(0x0046E310, MissionScript_AppendElement32, "auto");
