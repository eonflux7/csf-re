#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body (common ancestor teardown) shared by GUI controls.
CSF_FUNC(int, __fastcall, CGuiSlider_DestroyBody, 0x72CB70,
         (CGuiSlider* self, void* edx));

static CGuiSlider* __fastcall CGuiSlider_Destroy(
    CGuiSlider* self, void* /*edx*/, char flags)
{
    CGuiSlider_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x72D960, CGuiSlider_Destroy, "auto");
