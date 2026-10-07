#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiRadioGroup_DestroyBase, 0x72BB50,
         (CGuiRadioGroup* self, void* edx));

static CGuiRadioGroup* __fastcall CGuiRadioGroup_Destroy(
    CGuiRadioGroup* self, void* /*edx*/, char flags)
{
    self->vtbl = reinterpret_cast<void**>(0x8660C8);
    CGuiRadioGroup_DestroyBase(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x72E8D0, CGuiRadioGroup_Destroy, "auto");
