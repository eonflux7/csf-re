#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, ReleaseChildResource, 0x0081BD50, (uint32_t* a1));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor of C_00861b58 (derived from C_00860178).
// Resets the vtable chain, releases the owned child resource at +0x4,
// runs the base-class destructor, and frees the block when bit 0 of the flag is set.
static C_00861b58* __fastcall C_00861b58_DeletingDestructor(C_00861b58* self, void* /*edx*/, char deleteFlag)
{
    *(void***)self = CSF_VTBL(0x00861B58);

    uint32_t ownedResource = self->field_0004;
    if (ownedResource) {
        ReleaseChildResource(reinterpret_cast<uint32_t*>(ownedResource));
        self->field_0004 = 0;
    }

    *(void***)self = CSF_VTBL(0x00861A60);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((deleteFlag & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x6E4220, C_00861b58_DeletingDestructor, "auto");
