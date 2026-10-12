#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies a run of 16-byte field records into raw storage (cdecl, ret 0).
// The original pushes five stack arguments before the call and cleans them up with add esp, 0x14.
CSF_FUNC(uint32_t*, __cdecl, DataDefs_CopyFieldRecords,
         0x0055AB00,
         (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(unsigned int, __fastcall, DataDefs_InsertFieldRecords,
         0x0055ABF0,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// Appends one 16-byte field record to the vector-like container (__thiscall, ret 4).
static unsigned int __fastcall DataDefs_RegisterField(
    S_0055af00* self, void* /*edx*/, uint32_t* a2)
{
    const std::uint32_t begin = self->field_0004;

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0008 - begin) >> 4)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_000c - begin) >> 4))
    {
        return DataDefs_InsertFieldRecords(
            self, nullptr,
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            1,
            a2);
    }

    const std::uint32_t insertionAddress = self->field_0008;

    const unsigned int result = static_cast<unsigned int>(
        reinterpret_cast<std::uintptr_t>(DataDefs_CopyFieldRecords(
            reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(insertionAddress)),
            1,
            a2,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(a2)))));

    self->field_0008 = insertionAddress + 16;
    return result;
}

HOOK(0x0055AF00, DataDefs_RegisterField, "auto");
