#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(double, __cdecl, RpLightGetConeAngle, 0x0059CC50, (int light));
CSF_FUNC(int, __stdcall, sub_6E33E0, 0x006E33E0, (float a1, float a2, uint8_t* a3, float* a4));

static int __stdcall Im3dLight_DrawConeWithLtm(int light, uint8_t* a2)
{
    uint8_t* lightBytes = reinterpret_cast<uint8_t*>(light);

    float* ltm = reinterpret_cast<float*>(RwFrameGetLTM(CSF_AT(std::uint32_t, lightBytes, 0x4)));
    float v4 = CSF_AT(float, lightBytes, 0x14);
    float coneAngle = static_cast<float>(RpLightGetConeAngle(light));

    return sub_6E33E0(coneAngle, v4, a2, ltm);
}

HOOK(0x006E3A30, Im3dLight_DrawConeWithLtm, "auto");
