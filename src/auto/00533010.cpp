#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor body of CMenuEula (__thiscall, ret 0), invoked before the optional free.
CSF_FUNC(void, __fastcall, CMenuEula_DestroyBody, 0x532DC0,
         (CMenuEula* self, void* edx));

// Deleting destructor: run the destructor body, then free the object if the
// low bit of the flags argument is set. Always returns the object pointer.
static CMenuEula* __fastcall CMenuEula_Destroy(
    CMenuEula* self, void* /*edx*/, char flags)
{
    CMenuEula_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x533010, CMenuEula_Destroy, "auto");
