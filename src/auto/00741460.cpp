#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_68E2C0, 0x0068E2C0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, sub_68E2E0, 0x0068E2E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(double, __fastcall, CalculateSquaredVectorLength, 0x0068E300, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, sub_68E320, 0x0068E320, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(char, __fastcall, sub_68E360, 0x0068E360, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, sub_68E450, 0x0068E450, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, sub_7A2D60, 0x007A2D60, (void* self, void* edx, char a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MessageBoxCollapsedScale, 0x0082DFCC);
CSF_GLOBAL(float, g_AngleThreshold, 0x008678E8);

static void __fastcall CMenuMpStats_UpdateAimDirection(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    alignas(4) uint8_t vecBBytes[12];   // ebp-0x30
    alignas(4) uint8_t vecABytes[12];   // ebp-0x24
    alignas(4) uint8_t tmpABytes[12];   // ebp-0x18
    alignas(4) uint8_t tmpBBytes[12];   // ebp-0x0C

    uint32_t* vecB = reinterpret_cast<uint32_t*>(vecBBytes);
    uint32_t* vecA = reinterpret_cast<uint32_t*>(vecABytes);
    uint32_t* tmpA = reinterpret_cast<uint32_t*>(tmpABytes);
    uint32_t* tmpB = reinterpret_cast<uint32_t*>(tmpBBytes);

    if (!(Sub_64B250(self, nullptr) & 0xFF))
        return;

    uint8_t* member = reinterpret_cast<uint8_t*>(self) + 0x6F4;

    sub_68E2C0(vecB, nullptr, a2, a3, a4);
    sub_68E320(member, nullptr, reinterpret_cast<float*>(vecA), reinterpret_cast<float*>(vecB));
    double lengthSq = CalculateSquaredVectorLength(vecA, nullptr);

    if (lengthSq <= (double)g_fZero) {
        if (CSF_AT(uint8_t, self, 0x6F3) == 0) {
            if (lengthSq < (double)g_MessageBoxCollapsedScale)
                sub_7A2D60(self, nullptr, 2);
            CSF_AT(uint8_t, self, 0x6F3) = 1;
        }
    } else {
        CSF_AT(uint8_t, self, 0x6F3) = 0;
        sub_68E2E0(tmpB, nullptr, vecA);
        sub_68E2E0(tmpA, nullptr, reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x700));
        sub_68E360(tmpB, nullptr, 1.0f);
        sub_68E360(tmpA, nullptr, 1.0f);

        float* a = reinterpret_cast<float*>(tmpA);
        float* b = reinterpret_cast<float*>(tmpB);
        float dot = b[2] * b[2] + a[1] * b[1] + a[0] * b[0];

        if (dot <= g_TransitionOne) {
            double angle = CalculateArcCosine(dot);
            if (angle > (double)g_AngleThreshold)
                sub_7A2D60(self, nullptr, 3);
        }
        SceneLoader_NoOp();
        SceneLoader_NoOp();
    }

    sub_68E450(reinterpret_cast<uint8_t*>(self) + 0x1B8, nullptr, vecB);
    sub_68E2E0(reinterpret_cast<uint8_t*>(self) + 0x700, nullptr, vecA);
    sub_68E2E0(member, nullptr, vecB);
    SceneLoader_NoOp();
    SceneLoader_NoOp();
}

HOOK(0x00741460, CMenuMpStats_UpdateAimDirection, "auto");
