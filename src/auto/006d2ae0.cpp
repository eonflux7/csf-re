#include <cstdint>
#include "globals.h"
#include "hook.h"

// Clamps a float parameter to the camera's limit (field +0x48) and stores it
// in the sub-object referenced by field +0x0C at offset +0x88.
// Returns the sub-object pointer (+0x0C) as the result.
static int __fastcall SceneFrame_SetFloatParam(void* self, void* /*edx*/, float a2)
{
    float value = a2;
    if (a2 > CSF_AT(float, self, 0x48))
        value = CSF_AT(float, self, 0x48);

    uint32_t target = CSF_AT(uint32_t, self, 0x0C);
    CSF_AT(float, reinterpret_cast<void*>(target), 0x88) = value;
    return static_cast<int>(target);
}

HOOK(0x006D2AE0, SceneFrame_SetFloatParam, "auto");
