#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Vtable slot addresses for the two classes in this chain (C_0085e7b8 derives from C_0085e738).
// Stores are kept in the original order: derived vtable first, then base vtable.

// Destroys a C_0085e7b8 object: frees the owned block in field_0004, restores the
// base vtable, and frees the object itself when the low bit of releaseFlag is set.
// Returns the object's address (the vtable slot), matching the original's return value.
static uint32_t* __fastcall C_0085e7b8_Destroy(C_0085e7b8* self, void* /*edx*/, char releaseFlag)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085E7B8);

    uint32_t ownedBlock = self->field_0004;
    if (ownedBlock != 0)
    {
        CoreMemory_Free(reinterpret_cast<void*>(ownedBlock));
        self->field_0004 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0085E738);

    if ((releaseFlag & 1) != 0)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069A5D0, C_0085e7b8_Destroy, "auto");
