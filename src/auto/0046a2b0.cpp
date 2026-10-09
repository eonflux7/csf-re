#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a run of 16-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(uint32_t*, __cdecl, MissionScript_InitElementRange16_468B20,
         0x00468B20,
         (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(int, __fastcall, MissionScript_InsertElementCopies16_469D10,
         0x00469D10,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// Appends one 16-byte element to the vector-like container (__thiscall, ret 4).
static uint32_t* __fastcall MissionScript_AppendElement16(
    void* self, void* /*edx*/, uint32_t* source)
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
        const int insertResult = MissionScript_InsertElementCopies16_469D10(
            self, nullptr,
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(
                CSF_AT(std::uint32_t, self, 8))),
            1,
            source);
        return reinterpret_cast<uint32_t*>(
            static_cast<std::uintptr_t>(insertResult));
    }

    const std::uint32_t insertionAddress = CSF_AT(std::uint32_t, self, 8);

    uint32_t* const result = MissionScript_InitElementRange16_468B20(
        reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(insertionAddress)),
        1,
        source,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 16;
    return result;
}

HOOK(0x0046A2B0, MissionScript_AppendElement16, "auto");
