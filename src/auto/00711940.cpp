#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads the child pointer at +0x0C. If present, calls the child's vtable slot 4
// (offset 0x10) with a single flag argument of 1 and returns that result.
// Otherwise returns 0.
static int __fastcall C_00863d80_ActivateChildIfPresent(C_00863d80* self, void* /*edx*/)
{
    uint32_t childAddress = self->field_000c;
    if (childAddress == 0)
        return 0;

    void* child = reinterpret_cast<void*>(childAddress);

    // Callee is a method: `this` goes in ecx, one stack argument, callee pops it.
    using ChildActivateFn = int(__fastcall*)(void* self, void* edx, int flag);
    ChildActivateFn activate = reinterpret_cast<ChildActivateFn>(CSF_AT(void**, child, 0)[4]);
    return activate(child, nullptr, 1);
}

HOOK(0x711940, C_00863d80_ActivateChildIfPresent, "auto");
