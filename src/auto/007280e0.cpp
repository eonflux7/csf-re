#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sub-object destructor body, called first by the deleting-destructor wrapper.
CSF_FUNC(int, __fastcall, C_008655e8_DestroyBody, 0x727BD0,
         (C_008655e8* self, void* edx));

// Deleting destructor: runs the body, frees the object when the flags low bit is set,
// and returns the (possibly freed) object pointer.
static void* __fastcall C_008655e8_Destroy(
    C_008655e8* self, void* /*edx*/, char flags)
{
    C_008655e8_DestroyBody(self, nullptr);
    if ((flags & 1) != 0)
        game::Mem_Free(self);
    return self;
}

HOOK(0x7280E0, C_008655e8_Destroy, "auto");
