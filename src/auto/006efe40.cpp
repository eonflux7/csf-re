#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* /*edx*/, int a1));

// Clears base field 0x0C, calls virtual slot 10 on the embedded sub-object at +0x24
// with a2, then calls the same slot (with 1) on every child pointer in the array at +0xAC,
// up to the count at +0xB4 (re-read on each iteration).
static unsigned int __fastcall C_00862240_ResetAndNotifyChildren(C_00862240* self, void* /*edx*/, int a2)
{
    C_00860a30_ClearField000c(self, nullptr, a2);

    // Embedded sub-object at +0x24: its first dword is its vtable pointer; slot 0x28 -> index 10.
    void* embeddedChild = reinterpret_cast<uint8_t*>(self) + 0x24;
    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, embeddedChild, 0)[10])(embeddedChild, nullptr, a2);

    unsigned int result = self->field_00b4;
    for (unsigned int i = 0; i < result; ++i)
    {
        void* child = reinterpret_cast<void*>(
            static_cast<uintptr_t>(reinterpret_cast<uint32_t*>(self->field_00ac)[i]));
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[10])(child, nullptr, 1);
        result = self->field_00b4;
    }
    return result;
}

HOOK(0x006EFE40, C_00862240_ResetAndNotifyChildren, "auto");
