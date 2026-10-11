#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, S_006abb80_GetAxisBounds, 0x006AC000,
         (void* self, void* edx, float* a2, float* a3, float* a4,
          float* a5, float* a6, float* a7));
CSF_FUNC(float*, __fastcall, S_006abb80_SetAxisBounds, 0x006ABB80,
         (void* self, void* edx, float a2, float a3, float a4,
          float a5, float a6, float a7));

static float* __fastcall S_006abb80_IncludePointInBounds(
    S_006abb80* self, void* /*edx*/, float* point)
{
    float maxX, minX, maxY, minY, maxZ, minZ;
    S_006abb80_GetAxisBounds(self, nullptr, &maxX, &minX,
                            &maxY, &minY, &maxZ, &minZ);

    minX = minX < point[0] ? minX : point[0];
    maxX = maxX < point[0] ? point[0] : maxX;
    minY = minY < point[1] ? minY : point[1];
    maxY = maxY < point[1] ? point[1] : maxY;
    minZ = minZ < point[2] ? minZ : point[2];
    maxZ = maxZ < point[2] ? point[2] : maxZ;

    // The packet also lists this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x006ABB80);
    S_006abb80_SetAxisBounds(self, nullptr, maxX, minX,
                            maxY, minY, maxZ, minZ);
    return &self->field_0000;
}

HOOK(0x006AC6D0, S_006abb80_IncludePointInBounds, "auto");
