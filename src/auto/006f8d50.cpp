#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 44-byte record at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_006f8ce0_ConstructElement44, 0x006F8780,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 44-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_006f8ce0_InsertAndGetRecordPointer, 0x006F8CE0,
         (S_006f8ce0* self, void* /*edx*/, std::uint32_t* result, char* position, int* record));

// Appends one 44-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_006f8ce0_InsertAndGetRecordPointer.
static std::uint32_t* __fastcall S_006f8ce0_AppendElement44(
    S_006f8ce0* self, void* /*edx*/, int* record)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x006F8CE0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 44);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 44))
    {
        return S_006f8ce0_InsertAndGetRecordPointer(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&record),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            record);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_006f8ce0_ConstructElement44(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        reinterpret_cast<std::uint32_t*>(record),
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(record)));

    self->field_0008 = end + 44u;
    return result;
}

HOOK(0x006F8D50, S_006f8ce0_AppendElement44, "auto");
