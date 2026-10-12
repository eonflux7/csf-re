#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Restores the base vtable, frees the owned block at +0x14 if present, then
// re-stamps the C_00863070 vtable and runs the C_00860170 base constructor.
static void __fastcall C_008630e0_ReleaseBaseResources(C_008630e0* self, void* /*edx*/) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008630E0);
    if (self->field_0014) {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863070);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006FF970, C_008630e0_ReleaseBaseResources, "auto");
