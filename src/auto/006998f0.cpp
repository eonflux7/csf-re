#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Initializes a run of 8-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, CoreSortedTable_InitElementRange8_699180,
         0x00699180,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(void*, __fastcall, CoreSortedTable_InsertElementCopies8_699520,
         0x00699520,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

// Appends one 8-byte entry to the sorted table (__thiscall, ret 4).
static void* __fastcall CoreSortedTable_AppendEntry8(
    S_00699370* self, void* /*edx*/, uint32_t* source)
{
    const uint32_t begin = self->field_0004;

    const uint32_t entryCount = begin
        ? static_cast<uint32_t>(
              static_cast<std::int32_t>(self->field_0008 - begin) >> 3)
        : 0;

    if (!begin ||
        entryCount >= static_cast<uint32_t>(
            static_cast<std::int32_t>(self->field_000c - begin) >> 3))
    {
        return CoreSortedTable_InsertElementCopies8_699520(
            self, nullptr,
            static_cast<int>(self->field_0008),
            1,
            source);
    }

    const uint32_t insertionAddress = self->field_0008;

    void* result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        CoreSortedTable_InitElementRange8_699180(
            static_cast<int>(insertionAddress),
            1,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source)))));

    self->field_0008 = insertionAddress + 8;
    return result;
}

HOOK(0x006998F0, CoreSortedTable_AppendEntry8, "auto");
