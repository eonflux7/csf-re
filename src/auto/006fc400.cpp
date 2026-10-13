#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008655e8_SetCameraViewport, 0x006D29F0,
         (C_008655e8* self, void* /*edx*/, int left, int top, int width, int height));

// Stores the camera on this object. If a camera is present, applies the
// viewport rectangle held in this object's fields (left, top, right, bottom).
static int __fastcall C_00862e88_AttachCameraViewport(C_00862e88* self, void* /*edx*/, C_008655e8* camera)
{
    self->field_0010 = reinterpret_cast<uint32_t>(camera);
    if (camera) {
        int left = static_cast<int>(self->field_0014);
        int top = static_cast<int>(self->field_0018);
        int width = static_cast<int>(self->field_001c - self->field_0014);
        int height = static_cast<int>(self->field_0020 - self->field_0018);
        return C_008655e8_SetCameraViewport(camera, nullptr, left, top, width, height);
    }
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x6FC400, C_00862e88_AttachCameraViewport, "auto");
