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
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_008655e8_SetCameraViewport, 0x006D29F0, (C_008655e8* self, void* , int left, int top, int width, int height));

// Copies a 4-int rectangle (left, top, right, bottom) into the object, then if a
// camera viewport is attached, forwards it as (left, top, width, height).
// Without a viewport it returns the address of the stored left edge.
static int __fastcall C_00862e88_SetRectAndApplyViewport(C_00862e88* self, void* /*edx*/, int* rect)
{
    self->field_0014 = static_cast<uint32_t>(rect[0]);
    self->field_0018 = static_cast<uint32_t>(rect[1]);
    CSF_AT(int, self, 0x1C) = rect[2];
    CSF_AT(int, self, 0x20) = rect[3];

    int resultAddress = static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0014));

    C_008655e8* cameraViewport = reinterpret_cast<C_008655e8*>(self->field_0010);
    if (cameraViewport)
    {
        int left = static_cast<int>(self->field_0014);
        int top = static_cast<int>(self->field_0018);
        int width = CSF_AT(int, self, 0x1C) - left;
        int height = CSF_AT(int, self, 0x20) - top;
        return C_008655e8_SetCameraViewport(cameraViewport, nullptr, left, top, width, height);
    }
    return resultAddress;
}

HOOK(0x6FC440, C_00862e88_SetRectAndApplyViewport, "auto");
