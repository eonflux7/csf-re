#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, _floor, 0x007AFD90, (double X));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);

static uint32_t C_00832f18_GetPointCount(C_00832f18* self)
{
    return reinterpret_cast<uint32_t(__fastcall*)(void*, void*)>(
        self->vtbl[0x20 / 4])(self, nullptr);
}

static uint32_t C_00832f18_GetPlaybackMode(C_00832f18* self)
{
    return reinterpret_cast<uint32_t(__fastcall*)(void*, void*)>(
        self->vtbl[0x0C / 4])(self, nullptr);
}

static bool C_00832f18_IsPastEnd(C_00832f18* self, float time)
{
    return reinterpret_cast<bool(__fastcall*)(void*, void*, float)>(
        self->vtbl[0x10 / 4])(self, nullptr, time);
}

static float* C_00832f18_GetPoint(C_00832f18* self, uint32_t index)
{
    return reinterpret_cast<float*(__fastcall*)(void*, void*, uint32_t)>(
        self->vtbl[0x24 / 4])(self, nullptr, index);
}

static float C_00832f18_GetPointTime(C_00832f18* self, uint32_t index)
{
    return reinterpret_cast<float(__fastcall*)(void*, void*, uint32_t)>(
        self->vtbl[0x28 / 4])(self, nullptr, index);
}

static float* __fastcall C_00832f18_EvaluateHermitePosition(
    C_00832f18* self, void* /*edx*/, float* result, float time)
{
    const uint32_t pointCount = C_00832f18_GetPointCount(self);
    if (pointCount == 0)
    {
        result[0] = 0.0f;
        result[1] = 0.0f;
        result[2] = 0.0f;
        return result;
    }

    if (time < g_fZero)
    {
        float* point = C_00832f18_GetPoint(self, 0);
        result[0] = point[0];
        result[1] = point[1];
        result[2] = point[2];
        return result;
    }

    if (C_00832f18_IsPastEnd(self, time))
    {
        float* point = C_00832f18_GetPoint(self, pointCount - 1);
        result[0] = point[0];
        result[1] = point[1];
        result[2] = point[2];
        return result;
    }

    self->vSlot2C();

    if (C_00832f18_GetPlaybackMode(self) == 1)
    {
        const float duration = C_00832f18_GetPointTime(self, pointCount - 1);
        const int32_t cycle = static_cast<int32_t>(
            static_cast<int64_t>(_floor(time / duration)));
        time = time - static_cast<float>(cycle) * duration;
    }
    else if (C_00832f18_GetPlaybackMode(self) == 2)
    {
        const float duration = C_00832f18_GetPointTime(self, pointCount - 1);
        const int32_t cycle = static_cast<int32_t>(
            static_cast<int64_t>(_floor(time / duration)));
        if (cycle % 2 == 0)
        {
            time = time - static_cast<float>(cycle) * duration;
        }
        else
        {
            const int32_t nextCycle =
                static_cast<int32_t>(static_cast<uint32_t>(cycle) + 1);
            time = static_cast<float>(nextCycle) * duration - time;
        }
    }

    uint32_t firstIndex = 0;
    uint32_t secondIndex = 0;
    for (uint32_t index = 0; index < pointCount; ++index)
    {
        const uint32_t nextIndex = index + 1;
        if (nextIndex < pointCount)
        {
            const float nextTime = C_00832f18_GetPointTime(self, nextIndex);
            // Unordered comparisons also advance to the next segment.
            if (nextTime >= time)
            {
                firstIndex = index;
                secondIndex = nextIndex;
                break;
            }
        }
        else
        {
            firstIndex = index;
            secondIndex = index;
        }
    }

    float* point = C_00832f18_GetPoint(self, firstIndex);
    const float firstPoint[3] = {point[0], point[1], point[2]};

    point = C_00832f18_GetPoint(self, secondIndex);
    const float secondPoint[3] = {point[0], point[1], point[2]};

    if (firstIndex == secondIndex)
    {
        result[0] = firstPoint[0];
        result[1] = firstPoint[1];
        result[2] = firstPoint[2];
        return result;
    }

    const float* tangents = reinterpret_cast<const float*>(self->field_0034);
    const float* tangent = tangents + 3 * firstIndex;
    const float firstTangent[3] = {tangent[0], tangent[1], tangent[2]};
    tangent = tangents + 3 * secondIndex;
    const float secondTangent[3] = {tangent[0], tangent[1], tangent[2]};

    const float firstTime = C_00832f18_GetPointTime(self, firstIndex);
    const float elapsed = time - firstTime;
    const float secondTime = C_00832f18_GetPointTime(self, secondIndex);
    const float t = elapsed / (secondTime - firstTime);
    const float squared = t * t;
    const float cubed = squared * t;
    const float threeSquared = squared * g_MessageBoxSizeScale;

    const float firstWeight = cubed + cubed - threeSquared + g_TransitionOne;
    const float firstTangentWeight = cubed - (squared + squared) + t;
    const float secondWeight = cubed * g_RotationWrapStep + threeSquared;
    const float secondTangentWeight = cubed - squared;

    float interpolated[3];
    for (uint32_t component = 0; component < 3; ++component)
    {
        interpolated[component] =
            ((firstPoint[component] * firstWeight
              + firstTangent[component] * firstTangentWeight)
             + secondPoint[component] * secondWeight)
            + secondTangent[component] * secondTangentWeight;
    }

    result[0] = interpolated[0];
    result[1] = interpolated[1];
    result[2] = interpolated[2];
    return result;
}

HOOK(0x0047E350, C_00832f18_EvaluateHermitePosition, "auto");
