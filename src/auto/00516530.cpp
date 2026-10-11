#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Initializes a run of 16-byte records in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, MissionInfoPanel_InitRecordRange_515970,
         0x00515970,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the record storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(unsigned int, __fastcall, MissionInfoPanel_InsertRecordCopies_5160B0,
         0x005160B0,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// Appends one 16-byte record to the vector-like container (__thiscall, ret 4).
static unsigned int __fastcall MissionInfoPanel_AddRecord(
    S_00514aa0* self, void* /*edx*/, uint32_t* source)
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
        return MissionInfoPanel_InsertRecordCopies_5160B0(
            self, nullptr,
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            1u,
            source);
    }

    const std::uint32_t insertionAddress = self->field_0008;

    const unsigned int result = static_cast<unsigned int>(
        MissionInfoPanel_InitRecordRange_515970(
            static_cast<int>(insertionAddress),
            1,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source))));

    self->field_0008 = insertionAddress + 16;
    return result;
}

HOOK(0x00516530, MissionInfoPanel_AddRecord, "auto");
