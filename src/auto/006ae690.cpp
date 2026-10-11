#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Rebuilds the matrix state after the pitch angle changes.
// Original: 0x006AE570, __thiscall, no stack args.
CSF_FUNC(void, __fastcall, Rotation_RebuildMatrix, 0x006AE570, (void* self, void* edx));

// Adds a pitch delta to the angle at +0x004, then rebuilds the matrix.
static void __fastcall Matrix_AddPitchAngle(S_006ae570* self, void* /*edx*/, float pitchDelta)
{
    self->field_0004 = pitchDelta + self->field_0004;
    (void)CSF_ADDR_VT(0x006AE570); // packet lists this address as a vtable reference; no runtime effect
    Rotation_RebuildMatrix(self, nullptr);
}

HOOK(0x006AE690, Matrix_AddPitchAngle, "auto");
