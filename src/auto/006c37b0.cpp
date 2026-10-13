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

CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destroys the owned buffers and matrix, then re-runs the base-class constructor.
static void __fastcall C_00860848_Destroy(C_00860848* self, void* /*edx*/)
{
    void* ownedBlock = reinterpret_cast<void*>(self->field_0010);
    self->vtbl = CSF_VTBL(0x00860848);
    if (ownedBlock) {
        FreeBlock_690F70(ownedBlock);
        self->field_0010 = 0;
    }
    if (self->field_0004) {
        FreeBlock_690F70(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    std::uint32_t subObject = self->field_0030;
    if (subObject) {
        // Virtual slot +0x4 of the unknown sub-object: Release-style call with arg 1.
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, subObject, 0)[1])(
            reinterpret_cast<void*>(subObject), nullptr, 1);
        self->field_0030 = 0;
    }
    if (self->field_0028) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0024));
    }
    C_00860170_Construct((C_00860170*)self, nullptr);
}

HOOK(0x6C37B0, C_00860848_Destroy, "auto");
