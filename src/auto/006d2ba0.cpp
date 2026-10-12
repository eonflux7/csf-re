#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraSetProjection, 0x00628F90, (int a1, int a2));

static int __fastcall CCamera_SetProjectionType(CCamera* self, void* /*edx*/, int projectionType)
{
    self->field_004c = static_cast<uint32_t>(projectionType);
    if (projectionType == 0)
        return RwCameraSetProjection(static_cast<int>(self->field_000c), 1);
    if (projectionType == 1)
        return RwCameraSetProjection(static_cast<int>(self->field_000c), 2);
    return projectionType - 1;
}

HOOK(0x006D2BA0, CCamera_SetProjectionType, "auto");
