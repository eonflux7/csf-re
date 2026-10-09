#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, BuildQuerySegment, 0x0040FB60, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(char, __stdcall, IsObjectEligibleForQuery, 0x0044FFF0, (void* a1, int* a2));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxCollapsedScale, 0x0082DFCC);

static void __fastcall SceneQueries_UpdateClosestObjectHits(
    void* /*context*/, void* /*edx*/, float* results, int* query,
    void* origin, int collisionMask, C_008342e8* object)
{
    if (!object || !(object->field_0170 & 1))
        return;
    if (!object->vGetField016c())
        return;
    if (!object->field_004c)
        return;
    if (object->vSlot7C())
        return;

    if (object->field_0050 == 32)
    {
        void* related = reinterpret_cast<void*>(object->field_0034);
        if (related && !(CSF_AT(uint8_t, related, 0x40) & 1))
            return;
    }

    void* geometry = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(object->field_004c), nullptr));
    if (!geometry)
        return;
    if (!IsObjectEligibleForQuery(object, query))
        return;

    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));
    void* collisionScene = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr));

    const int flags = CSF_AT(int, query, 0);
    const bool querySecondary = (flags & 2) != 0;
    alignas(4) uint8_t hit[0x14]{};
    alignas(4) uint8_t facing[0x14]{};
    uint32_t segment[3]{};
    CSF_AT(int, hit, 0x0C) = 10;
    bool segmentTested = false;
    float distance = 0.0f;

    do
    {
        if (flags & 1)
        {
            const int requiredType = CSF_AT(int, query, 0x2C);
            float radius = 0.0f;
            if ((requiredType == -1 || requiredType == object->field_0050)
                && CSF_AT(float, query, 0x30) > g_fZero)
            {
                float* position = reinterpret_cast<float*>(
                    GetObjectPositionVector(object, nullptr));
                float* vector = BuildFacingVectorBetween(
                    reinterpret_cast<uint8_t*>(query) + 0x20, nullptr,
                    reinterpret_cast<float*>(facing), position);
                const double length = Vector_GetLength(vector, nullptr);
                radius = static_cast<float>(
                    std::sin(CSF_AT(float, query, 0x30) * g_HalfDurationScale)
                    * length * g_MessageBoxCollapsedScale);
            }

            if (((bool(__fastcall*)(void*, void*, void*, void*, float))
                    CSF_AT(void**, geometry, 0)[0x40 / 4])(
                    geometry, nullptr, origin, hit, radius))
            {
                float* vector = BuildFacingVectorBetween(
                    CSF_AT(void*, hit, 0), nullptr,
                    reinterpret_cast<float*>(facing),
                    reinterpret_cast<float*>(
                        reinterpret_cast<uint8_t*>(origin) + 0x0C));
                const double length = Vector_GetLength(vector, nullptr);
                distance = static_cast<float>(length);
                if (length < results[2])
                {
                    segmentTested = true;
                    BuildQuerySegment(
                        segment, nullptr,
                        reinterpret_cast<uint32_t*>(
                            reinterpret_cast<uint8_t*>(origin) + 0x0C),
                        CSF_AT(uint32_t*, hit, 0));
                    if (((bool(__fastcall*)(void*, void*, void*, int, int))
                            CSF_AT(void**, collisionScene, 0)[0x24 / 4])(
                            collisionScene, nullptr, segment, collisionMask, 3))
                        break;

                    CSF_AT(C_008342e8*, results, 0) = object;
                    results[2] = distance;
                    void* point = CSF_AT(void*, hit, 0);
                    results[4] = CSF_AT(float, point, 0x18);
                    results[5] = CSF_AT(float, point, 0);
                    results[6] = CSF_AT(float, point, 4);
                    results[7] = CSF_AT(float, point, 8);
                }
            }

            if (!querySecondary || !CSF_AT(uint8_t, hit, 0x10))
                break;
        }
        else if (!querySecondary)
        {
            break;
        }

        if (!((bool(__fastcall*)(void*, void*, void*, void*, uint32_t))
                CSF_AT(void**, geometry, 0)[0x44 / 4])(
                geometry, nullptr, origin, hit, 0))
            break;

        uint32_t* point = CSF_AT(uint32_t*, hit, 0);
        float* vector = BuildFacingVectorBetween(
            point, nullptr, reinterpret_cast<float*>(facing),
            reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(origin) + 0x0C));
        const double length = Vector_GetLength(vector, nullptr);
        distance = static_cast<float>(length);
        if (!(length < results[3]))
            break;

        if (!segmentTested)
        {
            BuildQuerySegment(
                segment, nullptr,
                reinterpret_cast<uint32_t*>(
                    reinterpret_cast<uint8_t*>(origin) + 0x0C),
                point);
            if (((bool(__fastcall*)(void*, void*, void*, int, int))
                    CSF_AT(void**, collisionScene, 0)[0x24 / 4])(
                    collisionScene, nullptr, segment, collisionMask, 3))
                break;
            point = CSF_AT(uint32_t*, hit, 0);
        }

        CSF_AT(C_008342e8*, results, 4) = object;
        results[3] = distance;
        CSF_AT(uint32_t, results, 0x20) = point[0];
        CSF_AT(uint32_t, results, 0x24) = point[1];
        CSF_AT(uint32_t, results, 0x28) = point[2];
    } while (false);

    ResetEmbeddedMemberAt8(reinterpret_cast<void**>(hit), nullptr);
}

HOOK(0x00451E40, SceneQueries_UpdateClosestObjectHits, "auto");
