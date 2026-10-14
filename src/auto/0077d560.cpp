#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 72-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, ConstructElement72_778B20, 0x00778B20,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 72-byte-element array and inserts one element (__thiscall, ret 4 from the caller side).
CSF_FUNC(uint32_t*, __fastcall, InsertElement72_77D4B0, 0x0077D4B0,
         (void* self, void* edx, uint32_t* a2, int a3, int a4));

// Appends one 72-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to InsertElement72_77D4B0.
static uint32_t* __fastcall GameDataDefs_AppendElement72(
    S_0077d4b0* self, void* /*edx*/, int element)
{
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] auto* const vtableRef = reinterpret_cast<void**>(CSF_ADDR_FN(0x0077D4B0));

    const uint32_t begin = self->field_0004;
    uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0008 - begin) / 72);
    }

    if (begin == 0 ||
        size >= static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - begin) / 72))
    {
        return InsertElement72_77D4B0(self, nullptr, reinterpret_cast<uint32_t*>(&element),
                                      self->field_0008, element);
    }

    const uint32_t end = self->field_0008;
    const int constructed = ConstructElement72_778B20(
        static_cast<int>(end), 1, element,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), element);

    self->field_0008 = end + 72;
    return reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(constructed));
}

HOOK(0x0077D560, GameDataDefs_AppendElement72, "auto");
