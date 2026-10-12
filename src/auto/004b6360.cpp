#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, BuildQuerySegment, 0x0040FB60, (uint32_t* self, void* , const uint32_t* first, const uint32_t* second));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiCore_CanReachPosition, 0x004AA390, (void* self, void* edx, float* arg0, float* a3));
CSF_FUNC(int, __stdcall, AiCore_TestQuerySegment, 0x004AA6A0, (int a1));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (S_004aa310* self, void* , int a2));
CSF_FUNC(char, __fastcall, TEstatico_CopyTargetPositionIfFree, 0x004AF540, (S_004af450* self, void* , float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);

static void* AiCore_GetLinkedBase(void* link)
{
    const int value = GetLinkedObjectValue_4DB5C0(link, nullptr);
    return value ? reinterpret_cast<void*>(static_cast<uint32_t>(value) - 4) : nullptr;
}

static void __fastcall AiCore_UpdateRadialSearch(float* self, void* /*edx*/, void* link)
{
    alignas(8) uint8_t queryBytes[0x18]{};
    alignas(8) uint8_t objectPointBytes[0xC]{};
    alignas(8) uint8_t candidateBytes[0xC]{};
    alignas(8) uint8_t adjustmentBytes[0xC]{};
    alignas(8) uint8_t endpointBytes[0xC]{};
    alignas(8) uint8_t targetBytes[0x20]{};

    auto* query = reinterpret_cast<float*>(queryBytes);
    auto* objectPoint = reinterpret_cast<float*>(objectPointBytes);
    auto* candidate = reinterpret_cast<float*>(candidateBytes);
    auto* adjustment = reinterpret_cast<float*>(adjustmentBytes);
    auto* endpoint = reinterpret_cast<float*>(endpointBytes);
    auto* target = reinterpret_cast<float*>(targetBytes);
    auto* origin = reinterpret_cast<float*>(targetBytes + 0xC);

    void* positionBase = AiCore_GetLinkedBase(link);
    void* validationBase = AiCore_GetLinkedBase(link);
    float* position = GetObjectPositionVector(
        CSF_AT(void*, positionBase, 0x2C), nullptr);
    if (!static_cast<uint8_t>(AiCore_IsTargetPositionValid(
            reinterpret_cast<S_004aa310*>(validationBase), nullptr,
            reinterpret_cast<int>(position))))
    {
        CSF_AT(int, self, 0x4) = 2;
        return;
    }

    void* base = AiCore_GetLinkedBase(link);
    if (CSF_AT(int, base, 0x608) == -1 &&
        CSF_AT(int, base, 0x60C) == -1)
    {
        CSF_AT(int, self, 0x4) = 3;
        return;
    }

    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x1C);
    base = AiCore_GetLinkedBase(link);
    position = GetObjectPositionVector(CSF_AT(void*, base, 0x2C), nullptr);
    base = AiCore_GetLinkedBase(link);
    void* object = CSF_AT(void*, base, 0x2C);
    ((float*(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, object, 0)[0xA4 / 4])(object, nullptr, objectPoint);

    origin[0] = position[0];
    origin[1] = position[1] + g_fReturnedConstant_0082E030;
    origin[2] = position[2];

    auto* entry = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(CSF_AT(void*, self, 0x14)) +
        28 * CSF_AT(uint32_t, self, 0x24));

    bool reached = false;
    if (!(entry[6] < g_fZero))
    {
        target[0] = origin[0] + entry[6] * entry[3];
        target[1] = origin[1] + entry[6] * entry[4];
        target[2] = origin[2] + entry[6] * entry[5];

        candidate[0] = objectPoint[0] + entry[6] * entry[3];
        candidate[1] = objectPoint[1] + entry[6] * entry[4];
        candidate[2] = objectPoint[2] + entry[6] * entry[5];

        if (entry[6] > g_fZero)
        {
            base = AiCore_GetLinkedBase(link);
            object = CSF_AT(void*, base, 0x2C);
            void* boundsOwner = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
            void* bounds = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, boundsOwner, 0)[0x28 / 4])(boundsOwner, nullptr);
            const float clearance = CSF_AT(float, bounds, 0xC) * g_HalfDurationScale;
            const float extendedDistance = clearance + entry[6];
            endpoint[0] = origin[0] + extendedDistance * entry[3];
            endpoint[1] = origin[1] + extendedDistance * entry[4];
            endpoint[2] = origin[2] + extendedDistance * entry[5];

            BuildQuerySegment(reinterpret_cast<uint32_t*>(query), nullptr,
                reinterpret_cast<const uint32_t*>(entry),
                reinterpret_cast<const uint32_t*>(endpoint));
            AiCore_GetLinkedBase(link);
            if (!static_cast<uint8_t>(
                    AiCore_TestQuerySegment(reinterpret_cast<int>(query))))
            {
                entry[6] = -1.0f;
            }
            else
            {
                endpoint[0] = 0.0f;
                endpoint[1] = 0.0f;
                endpoint[2] = 0.0f;
                base = AiCore_GetLinkedBase(link);
                if (TEstatico_CopyTargetPositionIfFree(
                        reinterpret_cast<S_004af450*>(base), nullptr, query,
                        reinterpret_cast<uint32_t*>(endpoint)))
                {
                    adjustment[0] = endpoint[0] - origin[0];
                    adjustment[1] = endpoint[1] - origin[1];
                    adjustment[2] = endpoint[2] - origin[2];
                    NormalizeVector3(
                        reinterpret_cast<C_00833b88*>(adjustment), nullptr);
                    adjustment[2] *= clearance;
                    target[0] = endpoint[0] - adjustment[0] * clearance;
                    target[2] = endpoint[2] - adjustment[2];
                    candidate[0] = target[0];
                    candidate[2] = target[2];
                    entry[6] = -1.0f;
                }
            }
        }

        base = AiCore_GetLinkedBase(link);
        const bool valid = static_cast<uint8_t>(AiCore_IsTargetPositionValid(
            reinterpret_cast<S_004aa310*>(base), nullptr,
            reinterpret_cast<int>(target))) != 0;
        if (!CSF_AT(uint32_t, self, 0x24) || valid)
        {
            base = AiCore_GetLinkedBase(link);
            adjustment[0] = CSF_AT(float, base, 0x64C);
            adjustment[1] = CSF_AT(float, base, 0x650);
            adjustment[2] = CSF_AT(float, base, 0x654);

            void* pointBase = AiCore_GetLinkedBase(link);
            void* ownerBase = AiCore_GetLinkedBase(link);
            void* pointObject = CSF_AT(void*, pointBase, 0x2C);
            void* ownerObject = CSF_AT(void*, ownerBase, 0x2C);
            float* returnedPoint = ((float*(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, pointObject, 0)[0xA4 / 4])(
                    pointObject, nullptr, endpoint);
            float* ownerPosition = GetObjectPositionVector(ownerObject, nullptr);
            adjustment[1] =
                returnedPoint[1] - ownerPosition[1] + adjustment[1];

            base = AiCore_GetLinkedBase(link);
            reached = AiCore_CanReachPosition(base, nullptr, candidate, adjustment);
            if (reached)
            {
                const uint32_t index = CSF_AT(uint32_t, self, 0x24);
                CSF_AT(int, self, 0x4) = 3;
                if (index)
                {
                    CSF_AT(float, self, 0x2C) = target[0];
                    CSF_AT(float, self, 0x30) = target[1];
                    CSF_AT(float, self, 0x34) = target[2];
                    CSF_AT(float, self, 0x8) = 5.0f;
                }
                else
                {
                    base = AiCore_GetLinkedBase(link);
                    position = GetObjectPositionVector(
                        CSF_AT(void*, base, 0x2C), nullptr);
                    CSF_AT(float, self, 0x2C) = position[0];
                    CSF_AT(float, self, 0x30) = position[1];
                    CSF_AT(float, self, 0x34) = position[2];
                }
            }
        }

        entry[0] = target[0];
        entry[6] = entry[6] + g_MaxPercent;
        entry[1] = target[1];
        entry[2] = target[2];
    }

    if (!reached)
    {
        const uint32_t nextIndex = CSF_AT(uint32_t, self, 0x24) + 1;
        CSF_AT(uint32_t, self, 0x24) = nextIndex;
        if (nextIndex >= entryCount)
        {
            CSF_AT(float, self, 0x28) += g_MaxPercent;
            if (CSF_AT(float, self, 0x28) <= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xAAC))
            {
                CSF_AT(uint32_t, self, 0x24) = 1;
            }
            else
            {
                CSF_AT(int, self, 0x4) = 2;
                CSF_AT(float, self, 0x8) = 5.0f;
            }
        }
    }
}

HOOK(0x004B6360, AiCore_UpdateRadialSearch, "auto");
