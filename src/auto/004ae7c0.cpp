#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Resets the vtable of the embedded sub-objects at +0x10, +0x08, +0x04 and of
// the object itself, then frees the block when bit 0 of the argument is set.
// Returns `this`.
static C_0082d410* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char freeMemory)
{
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    C_0082d410_SetVtable(self, nullptr);

    if ((freeMemory & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x004AE7C0, C_0082d410_Destroy, "auto");
