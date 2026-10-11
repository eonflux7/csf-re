#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clears the dword at +0x8 of the embedded object referenced by field_0004,
// after resetting this object's vtable to C_00867768's.
static void __fastcall MpSpawnPoints_DestroyEmbeddedObject(C_00867768* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00867768);

    uint32_t embeddedObjectAddr = self->field_0004;
    if (embeddedObjectAddr != 0) {
        void* embeddedObject = reinterpret_cast<void*>(embeddedObjectAddr);
        CSF_AT(uint32_t, embeddedObject, 0x8) = 0;
    }
}

HOOK(0x0073F710, MpSpawnPoints_DestroyEmbeddedObject, "auto");
