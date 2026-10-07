#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body called before the possible deallocation (0x72F140).
CSF_FUNC(void, __fastcall, CGuiChoiceButton_DestroyBody, 0x72F140,
         (CGuiChoiceButton* self, void* edx));

static CGuiChoiceButton* __fastcall CGuiChoiceButton_Destroy(
    CGuiChoiceButton* self, void* /*edx*/, char flags)
{
    CGuiChoiceButton_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x72F560, CGuiChoiceButton_Destroy, "auto");
