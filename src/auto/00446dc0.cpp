#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegreesToRadians, 0x008310F4);

CSF_FUNC(uint32_t*, __fastcall, sub_446C10, 0x00446C10, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

static int __fastcall GatherSceneCandidates(
    void* self, void* /*edx*/, int handle, S_00446340* candidates)
{
    candidates->field_0008 = 0;

    auto* object = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (object && (object->field_0170 & 0x2000) != 0)
    {
        alignas(8) uint8_t directionBytes[0x18];
        alignas(8) uint8_t facingBytes[0x8];
        alignas(8) uint8_t temporaryBytes[0x14];

        auto* direction = reinterpret_cast<float*>(directionBytes);
        auto* facing = reinterpret_cast<float*>(facingBytes);
        auto* temporary = reinterpret_cast<float*>(temporaryBytes);

        float* position = GetObjectPositionVector(object, nullptr);
        float* angles =
            (reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
                reinterpret_cast<void**>(object->vtbl)[0xAC / 4]))(
                    object, nullptr, temporary);
        GfxMatrix_EulerAnglesToDirection(angles, nullptr, direction);

        facing[0] = direction[0];
        facing[1] = direction[2];
        NormalizeVector2(facing, nullptr);

        void* settings = reinterpret_cast<void*>(object->field_003c);
        double angle =
            (reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, settings, 0)[0x7C / 4]))(
                    settings, nullptr);

        settings = reinterpret_cast<void*>(object->field_003c);
        float minimumDot = static_cast<float>(
            std::cos(angle * g_DegreesToRadians));
        float radius = CSF_AT(float, settings, 0x38);
        float radiusSquared = radius * radius;

        uint32_t remaining = CSF_AT(uint32_t, self, 0x14);
        uint32_t offset = 0;
        while (remaining != 0)
        {
            uint8_t* record = CSF_AT(uint8_t*, self, 0x0C) + offset;
            if (CSF_AT(int32_t, record, 0) != -1)
            {
                float dx = CSF_AT(float, record, 0x10) - position[0];
                float dy = CSF_AT(float, record, 0x14) - position[1];
                float dz = CSF_AT(float, record, 0x18) - position[2];

                if (dy * dy + dx * dx + dz * dz < radiusSquared)
                {
                    temporary[0] = dx;
                    temporary[1] = dz;
                    NormalizeVector2(temporary, nullptr);

                    if (temporary[1] * facing[1] +
                        temporary[0] * facing[0] >= minimumDot)
                    {
                        sub_446C10(candidates, nullptr,
                                   reinterpret_cast<uint32_t*>(record), 0);
                    }
                }
            }
            offset += 36;
            --remaining;
        }
    }

    return candidates->field_0008 != 0;
}

HOOK(0x00446DC0, GatherSceneCandidates, "auto");
