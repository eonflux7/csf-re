#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Body destructor (0x725590): releases the button's quads, text, fx and effect list.
CSF_FUNC(void, __fastcall, CGuiButton_DestroyBody, 0x725590,
         (CGuiButton* self, void* edx));

static CGuiButton* __fastcall CGuiButton_Destroy(CGuiButton* self, void* /*edx*/,
                                                 char flags)
{
    CGuiButton_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x725D20, CGuiButton_Destroy, "auto");
