#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuTextInput (0x52C2E0).
CSF_FUNC(int, __fastcall, CMenuTextInput_DestroyBody, 0x52C2E0,
         (CMenuTextInput* self, void* edx));

// Scalar deleting destructor: run the body, then free the object when the
// low bit of the flags argument is set (the "delete this" case).
static CMenuTextInput* __fastcall CMenuTextInput_Destroy(
    CMenuTextInput* self, void* /*edx*/, char flags)
{
    CMenuTextInput_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x52C350, CMenuTextInput_Destroy, "auto");
