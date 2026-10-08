#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CGuiControl (frees listeners/effects, destroys strings, etc.).
CSF_FUNC(void, __fastcall, CGuiControl_Destroy, 0x719B10, (CGuiControl* self, void* edx));

// Scalar-deleting destructor: run the base destructor, then free the block when
// the low bit of the flags argument is set; returns `this` unchanged.
static CGuiControl* __fastcall CGuiControl_DeletingDtor(
    CGuiControl* self, void* /*edx*/, char flags)
{
    CGuiControl_Destroy(self, nullptr);

    if ((flags & 1) != 0)
        game::Mem_Free(self);

    return self;
}

HOOK(0x719D80, CGuiControl_DeletingDtor, "auto");
