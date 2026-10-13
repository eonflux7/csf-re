#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CLightObject_GetLightTypeCode(CLightObject* self, void* /*edx*/)
{
    const auto light = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C));
    const int8_t lightType = CSF_AT(int8_t, light, 1);

    switch (lightType) {
    case 1:
        return 1;
    case 2:
        return 0;
    case static_cast<int8_t>(0x80):
        return 2;
    case -127: // Signed representation of 0x81.
        return 3;
    default:
        return 3;
    }
}

HOOK(0x006FCE20, CLightObject_GetLightTypeCode, "auto");
