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

// Original: void __thiscall sub_6D6AC0(C_008610c0* this), ret 0.
// Tears down the render handle and owned buffers, then reverts the object to its C_00860170 base.

CSF_FUNC(void, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, GfxScene_ResetRenderHandle_6D6760, 0x006D6760, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00863018_Destroy_6FEFF0, 0x006FEFF0, (C_00863018* self, void*));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static void __fastcall C_008610c0_TeardownAndRevertToBase(C_008610c0* self, void* /*edx*/)
{
    // Read the flag before the vtable is switched to the base class.
    uint8_t renderHandleActive = self->field_00d9;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861120);

    if (renderHandleActive) {
        GfxScene_ResetRenderHandle_6D6760(self, nullptr);
    }

    // Embedded sub-object at +0x74 (address, not a load).
    C_00863018_Destroy_6FEFF0(
        reinterpret_cast<C_00863018*>(reinterpret_cast<uint8_t*>(self) + 0x74), nullptr);

    if (self->field_0064) {
        Mem_Free(reinterpret_cast<void*>(self->field_0064));
        self->field_0064 = 0;
    }
    if (self->field_0054) {
        Mem_Free(reinterpret_cast<void*>(self->field_0054));
        self->field_0054 = 0;
    }
    if (self->field_0040) {
        Mem_Free(reinterpret_cast<void*>(self->field_0040));
        self->field_0040 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008610C0);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006D6AC0, C_008610c0_TeardownAndRevertToBase, "auto");
