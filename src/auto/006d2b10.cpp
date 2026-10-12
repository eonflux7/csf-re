#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraSetFarClipPlane, 0x00628EC0, (int a1, float a2));

// CCamera::SetFarClipPlane: stores the far clip distance in the camera object
// and forwards it to the RenderWare camera handle.
static int __fastcall CCamera_SetFarClipPlane(CCamera* self, void* /*edx*/, float farClipDistance)
{
    self->field_0048 = farClipDistance;
    return RwCameraSetFarClipPlane((int)self->field_000c, farClipDistance);
}

HOOK(0x006D2B10, CCamera_SetFarClipPlane, "auto");
