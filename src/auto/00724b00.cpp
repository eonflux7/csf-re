#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CGuiListBox (releases children/resources); __thiscall, no return value.
CSF_FUNC(void, __fastcall, CGuiListBox_Destroy_724720, 0x724720,
         (CGuiListBox* self, void* edx));

// Deleting destructor: run the destructor body, then free the object with the game heap
// when the low bit of the flags argument is set; returns the object pointer in eax.
static void* __fastcall CGuiListBox_Destroy(CGuiListBox* self, void* /*edx*/, char flags)
{
    CGuiListBox_Destroy_724720(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x724B00, CGuiListBox_Destroy, "auto");
