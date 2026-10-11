#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, RwMatrixInvert, 0x00624950, (float* a1, float* a2));

// Inverts the matrix pointed to by *matrixHandle into the target stored in this->field_0000.
static float* __fastcall Matrix_InvertWithStoredTarget(S_006adb40* self, void* /*edx*/, float** a2) {
    return RwMatrixInvert(*a2, reinterpret_cast<float*>(self->field_0000));
}

HOOK(0x006ADB40, Matrix_InvertWithStoredTarget, "auto");
