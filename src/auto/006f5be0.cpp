#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void* edx));

// Copies the three dwords referenced by args into the embedded vector object at self+0x20,
// normalizes that object in place, and sets the byte flag at self+0x3C to 1.
static float* __fastcall Projection_CopyVector3AndNormalize(uint8_t* self, void* /*edx*/, const uint32_t* args)
{
    C_00833b88* vectorObject = reinterpret_cast<C_00833b88*>(self + 0x20);
    CSF_AT(uint32_t, self, 0x20) = args[0];
    CSF_AT(uint32_t, self, 0x24) = args[1];
    CSF_AT(uint32_t, self, 0x28) = args[2];

    float* result = NormalizeVector3(vectorObject, nullptr);
    CSF_AT(uint8_t, self, 0x3C) = 1;
    return result;
}

HOOK(0x006F5BE0, Projection_CopyVector3AndNormalize, "auto");
