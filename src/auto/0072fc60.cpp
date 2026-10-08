#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00691110  int __cdecl Mem_Free(void*)
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* a1));

// 0x00725590  int __thiscall sub_725590(CGuiButton* this)  -- base (CGuiButton) destructor
CSF_FUNC(int, __fastcall, CGuiButton_Destroy_725590, 0x725590, (CGuiButton* self, void* edx));

// Scalar deleting destructor for CGuiRadioButton.
// - installs its own vtable (0x8668B8)
// - runs the base-class destructor (0x725590)
// - if the low bit of the flag argument is set, frees the object via the game heap
// - returns the object pointer
static CGuiRadioButton* __fastcall CGuiRadioButton_ScalarDeletingDestructor(
    CGuiRadioButton* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x8668B8);
    CGuiButton_Destroy_725590(self, nullptr);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x72FC60, CGuiRadioButton_ScalarDeletingDestructor, "auto");
