#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Releases the RwMatrix payload held at +0x1C, clears the cached track
// values at +0x10..+0x18, then re-runs the base C_00860170 constructor.
static void __fastcall C_00863360_ReleaseTrackPayload(C_00863360* self, void* /*edx*/) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008633A0);

    std::uint32_t matrixHandle = self->field_001c;
    if (matrixHandle != 0) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHandle));
        self->field_001c = 0;
    }

    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863360);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x00707550, C_00863360_ReleaseTrackPayload, "auto");
