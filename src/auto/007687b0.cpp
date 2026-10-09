#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees of CDummyMesh's reset/destroy path, bound by address.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the dummy-mesh entry list (releasing its buffer) and, when bit 0 of
// freeFlag is set, frees the CDummyMesh object itself. Returns the object.
static void** __fastcall CDummyMesh_Destroy(CDummyMesh* self, void* /*edx*/, char freeFlag) {
    self->vtbl = CSF_VTBL(0x0086F15C);

    if (self->m_dummiesBegin) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->m_dummiesBegin));
    }

    self->m_dummiesBegin = 0;
    self->m_dummiesEnd = 0;
    self->m_dummiesCapacity = 0;

    if ((freeFlag & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<void**>(self);
}

HOOK(0x007687B0, CDummyMesh_Destroy, "auto");
