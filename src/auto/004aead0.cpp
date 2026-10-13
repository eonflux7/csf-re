#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void* edx));

// Releases the buffer held at +0x04 (if any), resets the vtable, and frees the
// object itself when bit 0 of the flag argument is set. Returns self.
static C_0082d410* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char freeSelfFlag)
{
    if (self->field_0004 != 0)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    C_0082d410_SetVtable(self, nullptr);

    if ((freeSelfFlag & 1) != 0)
    {
        CoreMemory_Free(static_cast<void*>(self));
    }

    return self;
}

HOOK(0x004AEAD0, C_0082d410_Destroy, "auto");
