#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CGuiBuilder (frees owned control factories etc.).
CSF_FUNC(int, __fastcall, CGuiBuilder_Destroy_71E090, 0x71E090,
         (CGuiBuilder* self, void* edx));

// Scalar deleting destructor: run the destructor body, then free the object
// when the low bit of the flags argument is set.
static CGuiBuilder* __fastcall CGuiBuilder_Destroy(
    CGuiBuilder* self, void* /*edx*/, char flags)
{
    CGuiBuilder_Destroy_71E090(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x71F4B0, CGuiBuilder_Destroy, "auto");
