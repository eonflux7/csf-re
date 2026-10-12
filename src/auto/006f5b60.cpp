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

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

// Copies two 3-float vectors into the object (+0x14 and +0x20), normalizes the second one in place,
// and marks the object's flag byte at +0x3C.
static float* __fastcall GfxCameraProjection_SetViewVectors(void* self, void* /*edx*/,
                                                            const uint32_t* a2, const uint32_t* a3) {
    CSF_AT(uint32_t, self, 0x14) = a2[0];
    CSF_AT(uint32_t, self, 0x18) = a2[1];
    CSF_AT(uint32_t, self, 0x1C) = a2[2];

    CSF_AT(uint32_t, self, 0x20) = a3[0];
    CSF_AT(uint32_t, self, 0x24) = a3[1];
    CSF_AT(uint32_t, self, 0x28) = a3[2];

    float* result = NormalizeVector3(reinterpret_cast<C_00833b88*>(static_cast<uint8_t*>(self) + 0x20), nullptr);
    CSF_AT(uint8_t, self, 0x3C) = 1;
    return result;
}

HOOK(0x006F5B60, GfxCameraProjection_SetViewVectors, "auto");
