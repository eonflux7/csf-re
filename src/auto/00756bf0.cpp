#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bindings for original callees (bind-as signatures from the packet).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, C_0086a3c0_ReleaseResources, 0x007567E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008446cc_ResetEmbedded, 0x00756AC0, (void* self, void* edx));

// Teardown for C_0086a3d8: resets the vtable chain, releases the owned buffer
// when its capacity is at least 0x10, then resets the string-like state.
static int __fastcall C_0086a3d8_Destroy(C_0086a3d8* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0086A3D8);

    C_0086a3c0_ReleaseResources(reinterpret_cast<uint8_t*>(self) + 0x22A4, nullptr);

    if (self->field_22a0 >= 0x10 && self->field_228c != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_228c));
    }

    self->field_22a0 = 15;
    self->field_229c = 0;
    *reinterpret_cast<uint8_t*>(&self->field_228c) = 0;  // mov byte ptr [esi+0x228c], 0

    int result = C_008446cc_ResetEmbedded(reinterpret_cast<uint8_t*>(self) + 0x8, nullptr);

    self->vtbl = CSF_VTBL(0x008446CC);
    return result;
}

HOOK(0x756BF0, C_0086a3d8_Destroy, "auto");
