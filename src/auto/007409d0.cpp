#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_68E2C0, 0x0068E2C0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, sub_68E2E0, 0x0068E2E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, sub_7A2A40, 0x007A2A40, (void* self, void* edx, float* a2));
CSF_FUNC(double, __cdecl, sub_7A2AD0, 0x007A2AD0, (float a1));
CSF_FUNC(char, __fastcall, sub_7A2D90, 0x007A2D90, (void* self, void* edx, char a2));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

CSF_GLOBAL(float, g_fQuarterPi, 0x0082FAC8);

static int FloatBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static char __fastcall NetCogibleDDL_ApplyRotationSnapshot(void* self, void* /*edx*/, float a3, float a4, float a5)
{
    char result = Sub_64B250(self, nullptr);
    if (result)
    {
        // 0xC-byte stack object filled by sub_68E2C0; element [1] is read back below.
        alignas(8) uint8_t snapshotBytes[0xC];
        float* snapshot = reinterpret_cast<float*>(snapshotBytes);

        // Original order: a5 first, then a3, then a4; each result is stored as float.
        float v9 = static_cast<float>(sub_7A2AD0(a5));
        float v8 = static_cast<float>(sub_7A2AD0(a3));
        float v7 = static_cast<float>(sub_7A2AD0(a4));

        sub_68E2C0(snapshotBytes, nullptr, FloatBits(v7), FloatBits(v8), FloatBits(v9));

        // pi/4 threshold: the data at 0x82FAC8 holds 0.78539819f; the literal is the same value.
        const float quarterPiLiteral = 0.78539819f;
        const float quarterPiThreshold = (g_fQuarterPi > 0.0f) ? g_fQuarterPi : quarterPiLiteral;

        float yDelta = snapshot[1] - CSF_AT(float, self, 0x710);
        if (std::fabs(yDelta) > quarterPiThreshold)
        {
            sub_7A2D90(self, nullptr, static_cast<char>(3));
        }

        sub_7A2A40(static_cast<uint8_t*>(self) + 0x48, nullptr, snapshot);
        sub_68E2E0(static_cast<uint8_t*>(self) + 0x70C, nullptr, reinterpret_cast<uint32_t*>(snapshotBytes));
        SceneLoader_NoOp();
    }
    return result;
}

HOOK(0x007409D0, NetCogibleDDL_ApplyRotationSnapshot, "auto");
