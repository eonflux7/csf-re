#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: C_00860a30::SetFlag0C (__thiscall, one stack arg, ret 4).
CSF_FUNC(void, __fastcall, C_00860a30_SetFlag0C, 0x006C9FE0, (C_00860a30* self, void* /*edx*/, int flag));

// Virtual slot 0x2C (index 11) takes (this, flag) in the original's __thiscall shape.
using VirtualSetFlagFn = void(__fastcall*)(void* self, void* edx, int flag);

// Activates the embedded sub-object at +0x24 and every element in the pointer array
// at +0xAC (count at +0xB4) through virtual slot 0x2C, then sets flag 0x0C on this object.
static void __fastcall C_00862240_ActivateSubObjectsAndSetFlag(C_00862240* self, void* /*edx*/, int /*unused*/)
{
    // The embedded object lives at this+0x24. Its first dword (the value stored at +0x24)
    // is its vtable pointer, so slot 0x2C is vtbl[11]. The call passes &this->field_0024 as this.
    void** embeddedVtbl = CSF_AT(void**, self, 0x24);
    ((VirtualSetFlagFn)embeddedVtbl[11])(&self->field_0024, nullptr, 1);

    // Loop over element pointers. The count is re-read every iteration and the
    // comparison is unsigned (jb in the original).
    for (uint32_t i = 0; i < self->field_00b4; ++i)
    {
        // Re-read the array base each iteration: callees may change the object.
        void* element = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_00ac)[i]);

        // Element type is unknown: its vtable pointer is the first dword of the element.
        void** elementVtbl = CSF_AT(void**, element, 0);
        ((VirtualSetFlagFn)elementVtbl[11])(element, nullptr, 1);
    }

    C_00860a30_SetFlag0C(self, nullptr, 1);
}

HOOK(0x6EFEA0, C_00862240_ActivateSubObjectsAndSetFlag, "auto");
