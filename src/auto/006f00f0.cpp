#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860a30_ReleaseParentState, 0x006C9F40, (C_00860a30* self, void*));
CSF_FUNC(void, __fastcall, C_00862240_ReleaseSubobjectState, 0x006EFD60, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetAtomicState, 0x006F3A90, (C_00862748* self, void*));

// Resets C_00862240 to its base vtables, frees its two heap buffers,
// resets the embedded atomic state at +0x24, then releases the parent state.
static void __fastcall C_00862240_ReleaseAtomicResources(C_00862240* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862240);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862238)));
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0086221C)));
    self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862200)));

    C_00862240_ReleaseSubobjectState(self, nullptr);

    if (self->field_00bc)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_00bc));
        self->field_00bc = 0;
    }
    if (self->field_00ac)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_00ac));
        self->field_00ac = 0;
    }

    RpPrtStd_ResetAtomicState(
        reinterpret_cast<C_00862748*>(reinterpret_cast<uint8_t*>(self) + 0x24), nullptr);

    self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x008621DC)));

    // Tail jump in the original: forwards ecx=this to the parent release.
    C_00860a30_ReleaseParentState(self, nullptr);
}

HOOK(0x006F00F0, C_00862240_ReleaseAtomicResources, "auto");
