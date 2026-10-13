#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 44-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_0069f1e0_ConstructElement44, 0x0069EBC0,
         (int a1, int a2, void* a3, int a4, int a5));

// Grows the 44-byte-element array and inserts one element (__fastcall/__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_0069f1e0_InsertElement44, 0x0069F1E0,
         (void* self, void* edx, std::uint32_t* a2, void* a3, void* a4));

// Appends one 44-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_0069f1e0_InsertElement44.
static std::uint32_t* __fastcall S_0069f1e0_AppendElement44(
    S_0069f1e0* self, void* /*edx*/, std::uint32_t argument)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0069F1E0);

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
        return S_0069f1e0_InsertElement44(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&argument),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(argument)));
    }

    const std::uint32_t end = self->field_0008;
    const int constructed = S_0069f1e0_ConstructElement44(
        static_cast<int>(end), 1,
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(argument)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(argument));

    std::uint32_t* result = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(constructed)));

    self->field_0008 = end + 44u;
    return result;
}

HOOK(0x0069F250, S_0069f1e0_AppendElement44, "auto");
