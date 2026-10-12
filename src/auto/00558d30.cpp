#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Initializes a run of 8-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, MpRules_InitElementRange8_5585B0,
         0x005585B0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(unsigned int, __fastcall, MpRules_InsertElementCopies8_558890,
         0x00558890,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

// Appends one 8-byte element to the vector-like container (__thiscall, ret 4).
static unsigned int __fastcall MpRules_AppendElement8(
    S_005587c0* self, void* /*edx*/, uint32_t* source)
{
    const std::uint32_t begin = self->field_0004;

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0008 - begin) >> 3)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_000c - begin) >> 3))
    {
        return MpRules_InsertElementCopies8_558890(
            self, nullptr,
            static_cast<int>(self->field_0008),
            1,
            source);
    }

    const std::uint32_t insertionAddress = self->field_0008;

    const unsigned int result = static_cast<unsigned int>(
        MpRules_InitElementRange8_5585B0(
            static_cast<int>(insertionAddress),
            1,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source))));

    self->field_0008 = insertionAddress + 8;
    return result;
}

HOOK(0x00558D30, MpRules_AppendElement8, "auto");
