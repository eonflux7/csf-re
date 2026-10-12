#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453660, 0x00453660, (CGuiControl* self, void* , S_0041e5f0* a2, float* a3, int* a4, float a5));
CSF_FUNC(float*, __cdecl, WeaponFirearms_ComputeBallisticVelocity, 0x00478930, (float* velocity, float* origin, float* target));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiDistracted_IsPositionBlocked, 0x004A9F30, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(float, g_GravityAccel, 0x0082E0EC);
CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(float, g_DistractionTimeStep, 0x0083A8B0);
CSF_GLOBAL(float, g_DistractionHeightScale, 0x0083A8B4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static float AiDistracted_ReadFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t AiDistracted_FloatBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static char __fastcall S_004b1050_TryStartDistraction(
    S_004b1050* self, void* /*edx*/, float linkedObjectArgument)
{
    void* linkedHandle = reinterpret_cast<void*>(
        AiDistracted_FloatBits(linkedObjectArgument));

    alignas(8) uint8_t velocityBytes[0x10];
    float* velocity = reinterpret_cast<float*>(velocityBytes);
    alignas(8) uint8_t queryBytes[0x98];
    S_0041e5f0& query = *reinterpret_cast<S_0041e5f0*>(queryBytes);
    alignas(8) uint8_t filterBytes[0x4];
    int& filter = *reinterpret_cast<int*>(filterBytes);

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedHandle, nullptr);
    void* linkedBase = reinterpret_cast<void*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    uint32_t* position = GetObjectPositionVector(
        CSF_AT(void*, linkedBase, 0x2C), nullptr);
    query.field_005c = position[0];
    CSF_AT(uint32_t, &query, 0x60) = position[1];
    query.field_0064 = position[2];

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedHandle, nullptr);
    linkedBase = reinterpret_cast<void*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    void* linkedObject = CSF_AT(void*, linkedBase, 0x2C);
    void* objectProperties = CSF_AT(void*, linkedObject, 0x2C);

    query.field_0050 = CSF_AT(uint32_t, self, 0x14);
    query.field_0054 = CSF_AT(uint32_t, self, 0x18);
    CSF_AT(float, &query, 0x60) =
        CSF_AT(float, objectProperties, 0xE4) * g_DistractionHeightScale
        + CSF_AT(float, &query, 0x60);
    query.field_0058 = self->field_001c;

    WeaponFirearms_ComputeBallisticVelocity(
        velocity,
        reinterpret_cast<float*>(&query.field_005c),
        reinterpret_cast<float*>(&query.field_0050));

    void* globalState = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalState, nullptr));
    void* collisionWorld =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr);
    if (!collisionWorld)
        return 1;

    query.field_0044 = AiDistracted_FloatBits(velocity[0]);
    query.field_0048 = AiDistracted_FloatBits(velocity[1]);
    query.field_004c = AiDistracted_FloatBits(velocity[2]);

    float currentX = AiDistracted_ReadFloat(query.field_005c);
    float currentY = CSF_AT(float, &query, 0x60);
    float currentZ = AiDistracted_ReadFloat(query.field_0064);
    query.field_006c = query.field_005c;
    CSF_AT(uint32_t, &query, 0x70) = CSF_AT(uint32_t, &query, 0x60);
    query.field_0074 = query.field_0064;
    query.field_0078 = query.field_005c;
    query.field_007c = CSF_AT(uint32_t, &query, 0x60);
    CSF_AT(uint32_t, &query, 0x80) = query.field_0064;
    query.field_0068 = AiDistracted_FloatBits(30.0f);
    CSF_AT(float, &query, 0x84) = 30.0f;

    query.field_000c = 10;
    query.field_002c = 10;
    std::memset(&query, 0, 12);
    std::memset(&query.field_0020, 0, 12);
    std::memset(&query.field_0030, 0, 12);
    query.field_0010 = 0;
    std::memset(&query.field_0014, 255, 12);
    CSF_AT(int, &query, 0x3C) = -1;
    query.field_0040 = 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedHandle, nullptr);
    linkedBase = reinterpret_cast<void*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);
    uint32_t ignoredObject = CSF_AT(uint32_t, linkedBase, 0x2C);
    if (query.field_0028 >= query.field_0024)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(queryBytes + 0x20),
            nullptr, query.field_002c);
    }
    uint32_t* entry = reinterpret_cast<uint32_t*>(
        query.field_0020 + 4 * query.field_0028++);
    *entry = ignoredObject;

    uint32_t sceneContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (query.field_0028 >= query.field_0024)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(queryBytes + 0x20),
            nullptr, query.field_002c);
    }
    entry = reinterpret_cast<uint32_t*>(
        query.field_0020 + 4 * query.field_0028++);
    *entry = sceneContext;

    query.field_0010 = 18;
    filter = 2;
    bool hit = ((bool (__fastcall*)(void*, void*, uint32_t*, int*, int))
        CSF_AT(void**, collisionWorld, 0)[0x1C / 4])(
            collisionWorld, nullptr, &query.field_0078, &filter, 3);
    bool clear = !hit;
    if (!hit)
    {
        CGuiControl_ProcessDisplayFlags_453660(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
            &query, reinterpret_cast<float*>(&query.field_0078),
            reinterpret_cast<int*>(&query.field_0010), 0.0f);
        clear = query.field_0008 == 0;
    }

    float timeStep = 0.0f;
    unsigned int sampleCount = 0;
    while (clear)
    {
        query.field_0048 = AiDistracted_FloatBits(
            AiDistracted_ReadFloat(query.field_0048)
            - timeStep * g_GravityAccel);
        float zStep = AiDistracted_ReadFloat(query.field_004c) * timeStep;
        currentX = AiDistracted_ReadFloat(query.field_0044) * timeStep + currentX;
        currentY += AiDistracted_ReadFloat(query.field_0048) * timeStep;
        currentZ += zStep;

        float dx = currentX - AiDistracted_ReadFloat(query.field_006c);
        float dy = currentY - CSF_AT(float, &query, 0x70);
        float dz = currentZ - AiDistracted_ReadFloat(query.field_0074);
        float distance = std::sqrt(dz * dz + dy * dy + dx * dx)
                       - AiDistracted_ReadFloat(query.field_0068);
        if (distance >= g_QuarterTurnConstant)
        {
            CSF_AT(float, &query, 0x84) = distance;
            uint32_t sampleX = AiDistracted_FloatBits(currentX);
            query.field_007c = AiDistracted_FloatBits(currentY);
            query.field_0078 = sampleX;
            CSF_AT(float, &query, 0x80) = currentZ;

            dx = currentX - AiDistracted_ReadFloat(query.field_0050);
            dy = currentY - AiDistracted_ReadFloat(query.field_0054);
            dz = currentZ - AiDistracted_ReadFloat(query.field_0058);
            if (std::sqrt(dz * dz + dy * dy + dx * dx) < g_Const200_0f
                || static_cast<uint8_t>(AiDistracted_IsPositionBlocked(
                    g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(&query.field_0050))) != 0)
            {
                break;
            }

            filter = 2;
            hit = ((bool (__fastcall*)(void*, void*, uint32_t*, int*, int))
                CSF_AT(void**, collisionWorld, 0)[0x1C / 4])(
                    collisionWorld, nullptr, &query.field_0078, &filter, 3);
            clear = !hit;
            if (!hit)
            {
                CGuiControl_ProcessDisplayFlags_453660(
                    reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
                    &query, reinterpret_cast<float*>(&query.field_0078),
                    reinterpret_cast<int*>(&query.field_0010), 0.0f);
                clear = query.field_0008 == 0;
            }
            CSF_AT(float, &query, 0x70) = currentY;
            query.field_006c = sampleX;
            query.field_0074 = AiDistracted_FloatBits(currentZ);
            query.field_0068 = AiDistracted_FloatBits(distance);
            ++sampleCount;
        }
        timeStep += g_DistractionTimeStep;
        if (sampleCount >= 0x32)
            break;
    }

    if (query.field_0020)
    {
        Mem_Free(reinterpret_cast<void*>(query.field_0020));
        query.field_0020 = 0;
    }
    if (query.field_0000)
        Mem_Free(reinterpret_cast<void*>(query.field_0000));
    return static_cast<char>(clear);
}

HOOK(0x004B6A80, S_004b1050_TryStartDistraction, "auto");
