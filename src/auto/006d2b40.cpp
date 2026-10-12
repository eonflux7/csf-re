#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraSetNearClipPlane, 0x00628E90, (int a1, float a2));

// CCamera::SetNearClipPlane: stores the near clip distance in the camera object
// and forwards it to the RenderWare camera handle.
static int __fastcall CCamera_SetNearClipPlane(CCamera* self, void* /*edx*/, float nearClipDistance)
{
    self->field_0044 = nearClipDistance;
    return RwCameraSetNearClipPlane((int)self->field_000c, nearClipDistance);
}

HOOK(0x006D2B40, CCamera_SetNearClipPlane, "auto");
