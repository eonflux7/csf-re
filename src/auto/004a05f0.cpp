#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Release of the icon's active sub-resources (ALERTA/DUDOSO/PLANO icon state).
CSF_FUNC(int, __fastcall, ReleaseIconResources, 0x0049FFC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// C_008343a4::~C_008343a4 equivalent: resets vtable, releases sub-state, frees the two heap blocks.
static int __fastcall C_008343a4_Destroy(C_008343a4* self, void* /*edx*/)
{
    uint8_t hadActiveState = self->field_0004;
    *(void***)self = CSF_VTBL(0x008343A4);
    if (hadActiveState)
    {
        ReleaseIconResources(self, nullptr);
        self->field_0004 = 0;
    }
    if (self->field_0020)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0020));
        self->field_0020 = 0;
    }
    int result = static_cast<int>(self->field_0010);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }
    return result;
}

HOOK(0x4A05F0, C_008343a4_Destroy, "auto");
