#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Bindings for the original functions this one calls, by address.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Resets the light object's owned resources and rebinds its vtables/data
// pointers, then runs the base-class constructor as a tail call.
static void __fastcall C_00862df0_ReleaseResources(C_00862df0* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00862E88);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00862E7C)));
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00862E5C)));
    self->field_0010 = 0;
    self->field_0054 = 0;

    if (self->field_0058) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0058));
        self->field_0058 = 0;
    }

    if (self->field_0044) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0044));
        self->field_0044 = 0;
    }

    if (self->field_003c) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0038));
    }

    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    *(void***)self = CSF_VTBL(0x00862DF0);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00862DE8)));
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));

    // Original tail-jumps into the base constructor with ecx = this.
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006FC270, C_00862df0_ReleaseResources, "auto");
