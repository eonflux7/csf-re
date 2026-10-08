#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base destructor body (member cleanup) called before the optional heap free.
CSF_FUNC(int, __fastcall, CMenuMessageBox_DestructBody, 0x52AF50,
         (CMenuMessageBox* self, void* edx));

// Scalar deleting destructor: run the destructor body, then free the object
// when the low bit of the flags argument is set, returning the object pointer.
static CMenuMessageBox* __fastcall CMenuMessageBox_Destroy(
    CMenuMessageBox* self, void* /*edx*/, char flags)
{
    CMenuMessageBox_DestructBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x52AFF0, CMenuMessageBox_Destroy, "auto");
