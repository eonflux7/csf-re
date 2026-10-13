#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, Vec3_NormalizeHelper, 0x006AB830, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

static char __fastcall C_00862490_IsSegmentObstructed(
    C_00862490* self, void* /*edx*/, int targetAddress,
    unsigned int originAddress)
{
    const float* target = reinterpret_cast<const float*>(targetAddress);
    const float* origin = reinterpret_cast<const float*>(originAddress);

    alignas(8) uint8_t segmentBytes[0x18];
    alignas(8) uint8_t contextBytes[0x18];
    alignas(8) uint8_t startBytes[0xC];
    alignas(8) uint8_t displacementBytes[0xC];
    alignas(8) uint8_t directionBytes[0x14];
    alignas(8) uint8_t hitBytes[0xC];

    float* segment = reinterpret_cast<float*>(segmentBytes);
    float* start = reinterpret_cast<float*>(startBytes);
    float* displacement = reinterpret_cast<float*>(displacementBytes);
    float* direction = reinterpret_cast<float*>(directionBytes);
    float* hit = reinterpret_cast<float*>(hitBytes);

    direction[0] = target[0] - origin[0];
    direction[1] = target[1] - origin[1];
    direction[2] = target[2] - origin[2];

    const double length = RwV3dLength(direction);
    if (length < CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x54))
        return 0;

    Vec3_NormalizeHelper(direction, nullptr, static_cast<float>(length));

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* distanceProvider = CSF_AT(void*, globalState, 0xF0);
    const double startOffset =
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, distanceProvider, 0)[0x44 / 4])(
                distanceProvider, nullptr);

    displacement[1] = static_cast<float>(direction[1] * startOffset);
    displacement[2] = static_cast<float>(direction[2] * startOffset);
    start[0] = static_cast<float>(direction[0] * startOffset + origin[0]);
    start[1] = displacement[1] + origin[1];
    start[2] = displacement[2] + origin[2];

    const float endOffset =
        CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x54);
    segment[0] = start[0];
    segment[1] = start[1];
    segment[2] = start[2];

    CSF_AT(float, contextBytes, 0x10) = direction[1] * endOffset;
    CSF_AT(float, contextBytes, 0x14) = direction[2] * endOffset;
    displacement[0] = target[0] - direction[0] * endOffset;
    segment[3] = displacement[0];
    displacement[1] = target[1] - CSF_AT(float, contextBytes, 0x10);
    segment[4] = displacement[1];
    displacement[2] = target[2] - CSF_AT(float, contextBytes, 0x14);
    segment[5] = displacement[2];

    void* scene = reinterpret_cast<void*>(SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* collisionQuery =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, scene, 0)[0x3C / 4])(scene, nullptr);

    int queryFlags = 64;
    const char sceneHit =
        reinterpret_cast<char(__fastcall*)(void*, void*, uint8_t*, float*, int*, int)>(
            CSF_AT(void**, collisionQuery, 0)[0x10 / 4])(
                collisionQuery, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x50,
                segment, &queryFlags, 3);
    if (sceneHit)
        return sceneHit;

    displacement[0] = segment[3] - segment[0];
    displacement[1] = segment[4] - segment[1];
    displacement[2] = segment[5] - segment[2];
    const float lengthSquared =
        displacement[2] * displacement[2] +
        displacement[1] * displacement[1] +
        displacement[0] * displacement[0];

    SpatialQuery_BuildContext(
        reinterpret_cast<C_00833b88*>(contextBytes), nullptr,
        reinterpret_cast<uint32_t*>(startBytes),
        reinterpret_cast<uint32_t*>(displacementBytes));

    const uint32_t objectCount = CSF_AT(uint32_t, scene, 0xD0);
    for (uint32_t index = 0; index < objectCount; ++index)
    {
        void* object = CSF_AT(void**, scene, 0xC8)[index];
        const float metric = CSF_AT(float, object, 0x78);

        void* bounds =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
        const float radius = CSF_AT(float, bounds, 0xC);

        void* collisionObject =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x70 / 4])(object, nullptr);

        if (metric != g_MissingMetricValue &&
            radius * radius + lengthSquared > metric)
        {
            uint8_t* flags =
                reinterpret_cast<uint8_t*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);
            if ((*flags & 0x18) == 0 &&
                collisionObject &&
                CSF_AT(uint32_t, collisionObject, 0x4) &&
                reinterpret_cast<bool(__fastcall*)(void*, void*, uint8_t*, float*, int)>(
                    CSF_AT(void**, collisionObject, 0)[0x3C / 4])(
                        collisionObject, nullptr, contextBytes, hit, 0) &&
                hit[0] * hit[0] < lengthSquared)
            {
                return 1;
            }
        }
    }
    return sceneHit;
}

HOOK(0x006F1F00, C_00862490_IsSegmentObstructed, "auto");
