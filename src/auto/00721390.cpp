#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x721290: destructor body of CGuiEdit, tears down owned members.
CSF_FUNC(int, __fastcall, CGuiEdit_Destroy_721290, 0x721290,
         (CGuiEdit* self, void* edx));

// Scalar destructor of CGuiEdit: run the body, then release the object
// storage through the game heap when bit 0 of `flags` is set.
static void* __fastcall CGuiEdit_Destroy(CGuiEdit* self, void* /*edx*/, char flags)
{
    CGuiEdit_Destroy_721290(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x721390, CGuiEdit_Destroy, "auto");
