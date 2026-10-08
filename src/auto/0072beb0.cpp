#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body at 0x72BB50: tears down the panel's members.
CSF_FUNC(int, __fastcall, CGuiPanel_Destroy_72BB50, 0x72BB50,
         (CGuiPanel* self, void* edx));

// 0x72BEB0: CGuiPanel destructor. Runs the member-teardown body, then,
// when the low bit of the flag argument is set, releases the object memory.
static void* __fastcall CGuiPanel_Destroy(
    CGuiPanel* self, void* /*edx*/, char flags)
{
    CGuiPanel_Destroy_72BB50(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x72BEB0, CGuiPanel_Destroy, "auto");
