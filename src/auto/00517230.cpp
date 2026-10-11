#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 80-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_005170e0_ConstructElement80, 0x00516C70,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 80-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_005170e0_InsertElement80, 0x005170E0,
         (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 80-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_005170e0_InsertElement80.
static std::uint32_t* __fastcall S_005170e0_AppendElement80(
    S_005170e0* self, void* /*edx*/, int elementValue)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x005170E0);

    const std::uint32_t begin = self->field_0004;
    unsigned int slotIndex = 0;

    if (begin != 0)
    {
        slotIndex = static_cast<unsigned int>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 80);
    }

    if (begin == 0 ||
        slotIndex >= static_cast<unsigned int>(
                         static_cast<std::int32_t>(self->field_000c - begin) / 80))
    {
        int element = elementValue;
        return S_005170e0_InsertElement80(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            static_cast<int>(self->field_0008),
            elementValue);
    }

    const std::uint32_t end = self->field_0008;
    const int result = S_005170e0_ConstructElement80(
        static_cast<int>(end), 1, elementValue,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        elementValue);

    self->field_0008 = end + 80u;
    return reinterpret_cast<std::uint32_t*>(static_cast<std::intptr_t>(result));
}

HOOK(0x00517230, S_005170e0_AppendElement80, "auto");
