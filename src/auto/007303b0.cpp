#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CGuiSwitch destructor body (derived cleanup; frees child quads/text and calls base dtor).
CSF_FUNC(int, __fastcall, CGuiSwitch_DestroyBody, 0x72FCF0,
         (CGuiSwitch* self, void* edx));

// Vector deleting destructor: run the derived body, then free `this` if the low bit of the
// flags argument is set. Always returns `this`.
static void* __fastcall CGuiSwitch_Destroy(CGuiSwitch* self, void* /*edx*/, char flags)
{
    CGuiSwitch_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x7303B0, CGuiSwitch_Destroy, "auto");
