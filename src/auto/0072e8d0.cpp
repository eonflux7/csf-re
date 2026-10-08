#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiPanel_Destroy_72BB50, 0x72BB50,
         (CGuiRadioGroup* self, void* edx));

static CGuiRadioGroup* __fastcall CGuiRadioGroup_Destroy(
    CGuiRadioGroup* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x8660C8);
    CGuiPanel_Destroy_72BB50(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x72E8D0, CGuiRadioGroup_Destroy, "auto");
