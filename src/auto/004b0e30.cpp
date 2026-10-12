#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, AiStatic_InitializeSegment, 0x0040ECB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiStatic_TestSegment, 0x004AF540, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, AiStatic_TrySidePosition, 0x004B0B30, (void* self, void* edx, int arg0, float arg4));
CSF_FUNC(char, __fastcall, AiStatic_TryForwardPosition, 0x004B0D10, (void* self, void* edx, int arg0));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiStatic_SetSegmentEndpoints, 0x006AB460, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static float* __fastcall AiStatic_UpdateGuardPosition(float* self, void* /*edx*/, int actor)
{
    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x14)));

    if (target)
    {
        int linkedValue = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(actor), nullptr);
        void* linked = linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
        const std::int32_t candidate = static_cast<std::int32_t>(target->field_0078);
        void* linkedObject = CSF_AT(void*, linked, 0x2C);

        if (static_cast<std::uint8_t>(
                IsDistinctNonSentinelValue(linkedObject, nullptr, candidate)))
        {
            void* attached = reinterpret_cast<void*>(target->field_0038);
            if (attached &&
                !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, attached, 0)[0xB0 / 4])(attached, nullptr))
            {
                alignas(8) std::uint8_t segmentBytes[0x18];
                alignas(8) std::uint8_t alternateBytes[0xC];
                alignas(8) std::uint8_t hitBytes[0x18];
                alignas(8) std::uint8_t originBytes[0xC];
                alignas(8) std::uint8_t endpointBytes[0x14];

                auto* segment = reinterpret_cast<float*>(segmentBytes);
                auto* alternate = reinterpret_cast<float*>(alternateBytes);
                auto* hit = reinterpret_cast<float*>(hitBytes);
                auto* position = reinterpret_cast<float*>(hitBytes + 0xC);
                auto* origin = reinterpret_cast<float*>(originBytes);
                auto* endpoint = reinterpret_cast<float*>(endpointBytes);

                linkedValue = GetLinkedObjectValue_4DB5C0(
                    reinterpret_cast<void*>(actor), nullptr);
                linked = linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
                linkedObject = CSF_AT(void*, linked, 0x2C);
                reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
                    CSF_AT(void**, linkedObject, 0)[0xA4 / 4])(
                        linkedObject, nullptr, origin);

                float* source = GetObjectPositionVector(target, nullptr);
                float positionX = source[0];
                float positionY = source[1];
                position[0] = positionX;
                position[1] = positionY;
                position[2] = source[2];

                if ((target->field_0170 & 0x2000) != 0)
                {
                    source = reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
                        target->vtbl[0xA4 / 4])(target, nullptr, alternate);
                    positionX = source[0];
                    positionY = source[1];
                    position[0] = positionX;
                    position[1] = positionY;
                    position[2] = source[2];
                }

                endpoint[0] = position[0] - origin[0];
                endpoint[1] = position[1] - origin[1];
                endpoint[2] = position[2] - origin[2];
                const float distance = std::sqrt(
                    endpoint[2] * endpoint[2] +
                    endpoint[1] * endpoint[1] +
                    endpoint[0] * endpoint[0]);

                AiStatic_InitializeSegment(segment, nullptr);
                alternate[2] = endpoint[2] * distance;
                endpoint[0] = endpoint[0] * distance + origin[0];
                endpoint[1] = endpoint[1] * distance + origin[1];
                endpoint[2] = alternate[2] + origin[2];
                AiStatic_SetSegmentEndpoints(
                    segment, nullptr,
                    reinterpret_cast<uint32_t*>(origin),
                    reinterpret_cast<uint32_t*>(endpoint));

                hit[0] = 0.0f;
                hit[1] = 0.0f;
                hit[2] = 0.0f;
                linkedValue = GetLinkedObjectValue_4DB5C0(
                    reinterpret_cast<void*>(actor), nullptr);
                linked = linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;

                if (AiStatic_TestSegment(
                        linked, nullptr, segment, reinterpret_cast<uint32_t*>(hit)))
                {
                    position[0] = hit[0] - origin[0];
                    position[1] = hit[1] - origin[1];
                    position[2] = hit[2] - origin[2];
                    const double hitDistance = Vector_GetLength(position, nullptr);
                    if (distance * g_HalfDurationScale < hitDistance)
                    {
                        if (AiStatic_TryForwardPosition(self, nullptr, actor) ||
                            AiStatic_TrySidePosition(self, nullptr, actor, -1.0f) ||
                            AiStatic_TrySidePosition(self, nullptr, actor, 1.0f))
                        {
                            CSF_AT(int, self, 0x4) = 3;
                        }
                    }
                }
                else
                {
                    CSF_AT(float, self, 0x20) = positionX;
                    CSF_AT(float, self, 0x24) = positionY;
                    CSF_AT(float, self, 0x28) = position[2];
                }
            }
        }
    }

    if (CSF_AT(int, self, 0x4) != 3)
        CSF_AT(int, self, 0x4) = 2;
    return self;
}

HOOK(0x004B0E30, AiStatic_UpdateGuardPosition, "auto");
