#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(float*, __fastcall, AiStatic_InitializeSegment, 0x0040ECB0, (float* self, void*));
CSF_FUNC(int, __fastcall, S_0044d940_FindCachedPair, 0x0044DA80, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, S_0044d940_IsCachedPairValid, 0x0044DAF0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, S_0044d940_AddCachedPair, 0x0044DE90, (void* self, void* edx, int a2, int a3));
CSF_FUNC(bool, __stdcall, AiStatic_TestSegmentConnection, 0x0044DF20, (float* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static bool __fastcall S_0044d940_TestCachedSegmentConnection(
    S_0044d940* self, void* /*edx*/, int sourceHandle, int targetHandle, int cachePolicy)
{
    bool result = false;
    if (CSF_AT(int, reinterpret_cast<void*>(sourceHandle), 0x8) < 0 ||
        CSF_AT(int, reinterpret_cast<void*>(targetHandle), 0x8) < 0)
        return false;

    auto* source = reinterpret_cast<uint32_t*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, sourceHandle));
    auto* target = reinterpret_cast<uint32_t*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));

    if (source && target && CSF_AT(void*, source, 0x3C))
    {
        ++self->field_0024;
        int cacheIndex = S_0044d940_FindCachedPair(
            self, nullptr, sourceHandle, targetHandle);

        if (cacheIndex != -1)
        {
            const uint32_t cacheOffset = 12 * static_cast<uint32_t>(cacheIndex);
            if (S_0044d940_IsCachedPairValid(
                    self, nullptr,
                    static_cast<int>(self->field_000c + cacheOffset),
                    targetHandle, cachePolicy))
            {
                const uint8_t cachedResult = CSF_AT(
                    uint8_t,
                    reinterpret_cast<void*>(self->field_000c + cacheOffset),
                    0x8);
                ++self->field_0028;
                return cachedResult != 0;
            }
        }

        const unsigned int pointCount =
            reinterpret_cast<unsigned int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, target, 0)[60 / 4])(target, nullptr);

        if (pointCount)
        {
            alignas(8) uint8_t segmentBytes[0x18];
            alignas(8) uint8_t targetPointBytes[0xC];
            alignas(8) uint8_t sourcePositionBytes[0x1C];
            auto* segment = reinterpret_cast<float*>(segmentBytes);

            void* sourceObject = CSF_AT(void*, source, 0x3C);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, sourceObject, 0)[136 / 4])(
                    sourceObject, nullptr, sourcePositionBytes);

            AiStatic_InitializeSegment(segment, nullptr);
            segment[2] = CSF_AT(float, sourcePositionBytes, 0x8);
            segment[0] = CSF_AT(float, sourcePositionBytes, 0x0);
            segment[1] = CSF_AT(float, sourcePositionBytes, 0x4);

            unsigned int pointIndex = 0;
            do
            {
                if (result)
                    break;

                float* point =
                    reinterpret_cast<float*(__fastcall*)(void*, void*, void*, unsigned int)>(
                        CSF_AT(void**, target, 0)[64 / 4])(
                            target, nullptr, targetPointBytes, pointIndex);
                segment[3] = point[0];
                segment[4] = point[1];
                segment[5] = point[2];
                result = AiStatic_TestSegmentConnection(segment, source, target);
                ++pointIndex;
            }
            while (pointIndex < pointCount);
        }

        if (cacheIndex == -1)
            cacheIndex = S_0044d940_AddCachedPair(
                self, nullptr, sourceHandle, targetHandle);

        const uint32_t cacheOffset = 12 * static_cast<uint32_t>(cacheIndex);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c + cacheOffset), 0x4) =
            self->field_0004;
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c + cacheOffset), 0x8) =
            result;
    }

    return result;
}

HOOK(0x0044E1C0, S_0044d940_TestCachedSegmentConnection, "auto");
