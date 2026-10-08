#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiTextureListBox_Destroy_7312C0, 0x7312C0,
         (CGuiTextureListBox* self, void* edx));

static void* __fastcall CGuiTextureListBox_Destroy(
    CGuiTextureListBox* self, void* /*edx*/, char flags)
{
    CGuiTextureListBox_Destroy_7312C0(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x731410, CGuiTextureListBox_Destroy, "auto");
