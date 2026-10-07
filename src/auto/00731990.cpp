#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Iterates the element array at +0x08 (childCount entries at +0x10) and, for every
// non-null element, forwards the argument to the element's virtual method at
// vtbl byte offset 8. Returns the last virtual call's result, or the array address
// when no element invoked the method (matching the original's EAX usage).
static int __fastcall C_00867078_DispatchToChildren(
    C_00867078* self, void* /*edx*/, int argument)
{
    const uint32_t childCount = self->field_0010;
    int result; // Original leaves EAX untouched when the loop body never runs.

    for (uint32_t index = 0; index < childCount; ++index)
    {
        const uint32_t arrayAddress = self->field_0008;
        result = static_cast<int>(arrayAddress);

        void* child = reinterpret_cast<void**>(arrayAddress)[index];
        if (child != nullptr)
        {
            // vtable slot 2 == byte offset 8, a __thiscall (int) method.
            result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, child, 0)[2])(child, nullptr, argument);
        }
    }

    return result;
}

HOOK(0x00731990, C_00867078_DispatchToChildren, "auto");
