#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Releases the RenderWare matrix held in field_001c, clears the three
// cached values, re-runs the base constructor, and frees the object when
// the low bit of the argument is set (scalar-deleting-destructor style).
static int* __fastcall C_008633a0_ResetAndDestroy(C_008633a0* self, void* /*edx*/, char shouldFree)
{
    std::uint32_t matrixHandle = self->field_001c;
    self->vtbl = CSF_VTBL(0x008633A0);
    if (matrixHandle != 0) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHandle));
        self->field_001c = 0;
    }
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->vtbl = CSF_VTBL(0x00863360);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
    if ((shouldFree & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<int*>(self);
}

HOOK(0x707640, C_008633a0_ResetAndDestroy, "auto");
