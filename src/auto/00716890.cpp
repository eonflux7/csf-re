#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_DestroyBody, 0x716110,
         (CGuiListBoxBase* self, void* edx));

static void* __fastcall CGuiListBoxBase_Destroy(
    CGuiListBoxBase* self, void* /*edx*/, char flags)
{
    CGuiListBoxBase_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x716890, CGuiListBoxBase_Destroy, "auto");
