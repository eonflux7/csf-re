#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00691110  int __cdecl Mem_Free(void* a1)
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* a1));

// 0x00725590  int __thiscall CGuiButton_Destroy_725590(CGuiButton* this)
CSF_FUNC(int, __fastcall, CGuiButton_Destroy_725590, 0x725590, (CGuiButton* self, void* edx));

// Scalar deleting destructor for CGuiChoiceNumButton (BOTON_CHOICE_NUM, vtable 0x866600).
// - installs the derived vtable pointer (0x866600)
// - runs the CGuiButton base cleanup at 0x725590
// - if the low bit of the flag argument is set, releases the object with the game heap
// - returns the object pointer
static CGuiChoiceNumButton* __fastcall CGuiChoiceNumButton_ScalarDeletingDestructor(
    CGuiChoiceNumButton* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x866600);
    CGuiButton_Destroy_725590(self, nullptr);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x72FA90, CGuiChoiceNumButton_ScalarDeletingDestructor, "auto");
