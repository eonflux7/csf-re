#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body (frees text/quad resources etc.), called by the scalar deleting destructor.
CSF_FUNC(int, __fastcall, CGuiStatic_DestroyBody, 0x7209F0,
         (CGuiStatic* self, void* edx));

static void* __fastcall CGuiStatic_Destroy(CGuiStatic* self, void* /*edx*/, char flags)
{
    CGuiStatic_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x720EB0, CGuiStatic_Destroy, "auto");
