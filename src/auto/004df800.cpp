#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, GetEndpointRestrictionFlags, 0x004DF140,
         (int a1, bool* a2, bool* a3));
CSF_FUNC(int, __cdecl, CalculateSegmentIntersectionParameters, 0x004DF240,
         (float a1, float a2, float a3, float a4, float a5, float a6,
          float a7, float a8, float* a9, float* a10));
CSF_FUNC(int, __fastcall, GetSegmentEndpoints, 0x004E0B70,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0,
         (C_00833b88* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);

static int* __fastcall S_004dfbc0_UpdateSegmentCrossingPoints(
    S_004dfbc0* self, void* /*edx*/, int* segments)
{
    (void)self;

    alignas(8) uint8_t startBytes[0x10];
    alignas(8) uint8_t endBytes[0xC];
    alignas(8) uint8_t deltaBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t fromStartBytes[0x54];
    alignas(8) uint8_t fromEndBytes[0xC];
    alignas(8) uint8_t parameterBytes[0x6];
    alignas(8) uint8_t otherParameterBytes[0x4];

    float* start = reinterpret_cast<float*>(startBytes);
    float* end = reinterpret_cast<float*>(endBytes);
    float* delta = reinterpret_cast<float*>(deltaBytes);
    float* direction = reinterpret_cast<float*>(directionBytes);
    float* fromStart = reinterpret_cast<float*>(fromStartBytes);
    float* fromEnd = reinterpret_cast<float*>(fromEndBytes);
    float& parameter = *reinterpret_cast<float*>(parameterBytes);

    uint32_t index = 0;
    uint32_t byteOffset = 0;
    if (static_cast<uint32_t>(segments[2]) - 2 == 0)
        return segments;

    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(segments[0]) + byteOffset;
        if ((CSF_AT(uint8_t, entry, 0x24) & 3) == 0)
        {
            delta[0] = delta[1] = delta[2] = 0.0f;
            start[0] = start[1] = start[2] = 0.0f;
            end[0] = end[1] = end[2] = 0.0f;

            GetSegmentEndpoints(
                reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x14)),
                nullptr,
                reinterpret_cast<uint32_t*>(startBytes),
                reinterpret_cast<uint32_t*>(endBytes));

            delta[0] = end[0] - start[0];
            delta[1] = end[1] - start[1];
            delta[2] = end[2] - start[2];

            float crossingDeltaZ =
                CSF_AT(float, entry, 0x34) - CSF_AT(float, entry, 0xC);
            float crossingDeltaX =
                CSF_AT(float, entry, 0x2C) - CSF_AT(float, entry, 0x4);
            CalculateSegmentIntersectionParameters(
                start[0], start[2], delta[0], delta[2],
                CSF_AT(float, entry, 0x4), CSF_AT(float, entry, 0xC),
                crossingDeltaX, crossingDeltaZ, &parameter,
                reinterpret_cast<float*>(otherParameterBytes));

            if (parameter < g_fZero)
                parameter = 0.0f;
            else if (parameter > g_TransitionOne)
                parameter = 1.0f;

            float* crossing = reinterpret_cast<float*>(entry + 0x18);
            int segment = CSF_AT(int, entry, 0x14);
            crossing[0] = delta[0] * parameter + start[0];
            crossing[1] = delta[1] * parameter + start[1];
            crossing[2] = delta[2] * parameter + start[2];

            bool restrictEnd;
            bool restrictStart;
            uint8_t linkedRestricted = static_cast<uint8_t>(
                GetEndpointRestrictionFlags(
                    CSF_AT(int, reinterpret_cast<void*>(segment), 0xC),
                    &restrictEnd, &restrictStart));
            uint8_t segmentRestricted = static_cast<uint8_t>(
                GetEndpointRestrictionFlags(
                    segment, &restrictEnd, &restrictStart));

            if (segmentRestricted || linkedRestricted)
            {
                float halfLength = static_cast<float>(
                    RwV3dLength(delta) * g_HalfDurationScale);
                direction[0] = delta[0];
                direction[1] = delta[1];
                direction[2] = delta[2];
                NormalizeVector3(
                    reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

                if (halfLength < g_QuarterTurnConstant)
                {
                    crossing[0] = direction[0] * halfLength + start[0];
                    crossing[1] = direction[1] * halfLength + start[1];
                    crossing[2] = direction[2] * halfLength + start[2];
                }
                else
                {
                    bool moveFromStart = false;
                    if (restrictStart)
                    {
                        fromStart[0] = crossing[0] - start[0];
                        fromStart[1] = crossing[1] - start[1];
                        fromStart[2] = crossing[2] - start[2];
                        moveFromStart =
                            RwV3dLength(fromStart) < g_QuarterTurnConstant;
                    }

                    if (moveFromStart)
                    {
                        crossing[0] =
                            direction[0] * g_QuarterTurnConstant + start[0];
                        crossing[1] =
                            direction[1] * g_QuarterTurnConstant + start[1];
                        crossing[2] =
                            direction[2] * g_QuarterTurnConstant + start[2];
                    }
                    else if (restrictEnd)
                    {
                        fromEnd[0] = end[0] - crossing[0];
                        fromEnd[1] = end[1] - crossing[1];
                        fromEnd[2] = end[2] - crossing[2];
                        if (RwV3dLength(fromEnd) < g_QuarterTurnConstant)
                        {
                            crossing[0] =
                                end[0] - direction[0] * g_QuarterTurnConstant;
                            crossing[1] =
                                end[1] - direction[1] * g_QuarterTurnConstant;
                            crossing[2] =
                                end[2] - direction[2] * g_QuarterTurnConstant;
                        }
                    }
                }
            }
        }

        byteOffset += 20;
        ++index;
    }
    while (index < static_cast<uint32_t>(segments[2]) - 2);

    return segments;
}

HOOK(0x004DF800, S_004dfbc0_UpdateSegmentCrossingPoints, "auto");
