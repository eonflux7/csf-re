#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 68-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(void*, __cdecl, S_0075b5e0_ConstructElement68, 0x0075B090,
         (std::uint32_t* a1, int a2, int a3, int a4, int a5));

// Grows the 68-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_0075b5e0_InsertElement68, 0x0075B5E0,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int a4));

// Appends one 68-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_0075b5e0_InsertElement68.
static void* __fastcall S_0075b5e0_AppendElement68(
    S_0075b5e0* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0075B5E0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 68);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 68))
    {
        return reinterpret_cast<void*>(S_0075b5e0_InsertElement68(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element));
    }

    const std::uint32_t end = self->field_0008;
    void* result = S_0075b5e0_ConstructElement68(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);

    self->field_0008 = end + 68u;
    return result;
}

HOOK(0x0075B680, S_0075b5e0_AppendElement68, "auto");
