#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpCollisionWorldForAllIntersections, 0x0056BAF0, (int world, void* box, uintptr_t callback, void* userData));
CSF_FUNC(int, __cdecl, RwV3dTransformVector, 0x00623070, (int a1, int a2, int a3));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00867358_ClearFields, 0x006C3620, (C_00867358* self, void*));
CSF_FUNC(int, __fastcall, GfxObjectInstances_RegisterInstance, 0x007012E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxObjectInstances_TouchCachedEntry, 0x007013B0, (void* self, void* edx, int a2));

CSF_GLOBAL(double, g_ProjectionHeightOffset, 0x00863170);
CSF_GLOBAL(double, g_ProjectionRayDistance, 0x00863178);

// Preserve the packet's address bindings for these call-site annotations.
[[maybe_unused]] static const auto kCachedEntryBinding = CSF_VTBL(0x007013B0);
[[maybe_unused]] static const auto kArrayGrowthBinding = CSF_VTBL(0x0041E5F0);

static C_00863ac0* __fastcall GfxObjectInstances_UpdateProjection(
    void* self, void* /*edx*/,
    int objectAddress,
    float* origin,
    float radius,
    float length,
    int flags,
    S_007013b0* instances,
    float scale)
{
    // The collision callback receives the adjoining locals as one context.
    alignas(8) uint8_t frame[0xA4];
    auto* unitVector = reinterpret_cast<float*>(frame + 0x0C);
    auto* context = frame + 0x18;
    auto* direction = reinterpret_cast<float*>(frame + 0x20);
    auto* intersection = frame + 0x7C;
    auto* scratch = reinterpret_cast<float*>(frame + 0x98);

    const uintptr_t owner = reinterpret_cast<uintptr_t>(self);
    CSF_AT(uintptr_t, context, 0x54) = owner;

    int entryIndex = -1;
    if (CSF_AT(uint32_t, self, 0x14) != 0)
        entryIndex = *reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x0C));
    CSF_AT(uint32_t, self, 0x14) = 0;

    uintptr_t cachedProjection = 0;
    if (entryIndex != -1)
    {
        auto* entry = reinterpret_cast<uint8_t*>(instances->field_0008)
                    + static_cast<uint32_t>(entryIndex) * 16;
        if (CSF_AT(uintptr_t, entry, 0x00) == owner)
        {
            cachedProjection = CSF_AT(uintptr_t, entry, 0x0C);
            CSF_AT(uint32_t, entry, 0x08) = instances->field_0034;
            if (CSF_AT(int, entry, 0x04) != -1)
            {
                GfxObjectInstances_TouchCachedEntry(
                    instances, nullptr, reinterpret_cast<int>(entry));
            }
        }
    }

    const bool registeredNewEntry = cachedProjection == 0;
    if (registeredNewEntry)
    {
        entryIndex = GfxObjectInstances_RegisterInstance(
            instances, nullptr, reinterpret_cast<int>(self));
        if (entryIndex == -1)
            return reinterpret_cast<C_00863ac0*>(entryIndex);
    }

    auto* entryIndices = reinterpret_cast<S_0041e5f0*>(
        static_cast<uint8_t*>(self) + 0x0C);
    if (CSF_AT(uint32_t, self, 0x14) >= CSF_AT(uint32_t, self, 0x10))
    {
        S_0041e5f0_GrowArrayCapacity(
            entryIndices, nullptr, entryIndices->field_000c);
    }

    const uint32_t indexCount = entryIndices->field_0008;
    auto* indexSlot = reinterpret_cast<int*>(entryIndices->field_0000) + indexCount;
    entryIndices->field_0008 = indexCount + 1;
    *indexSlot = entryIndex;

    if (registeredNewEntry)
    {
        auto* entry = reinterpret_cast<uint8_t*>(instances->field_0008)
                    + static_cast<uint32_t>(entryIndex) * 16;
        cachedProjection = 0;
        if (CSF_AT(uintptr_t, entry, 0x00) == owner)
        {
            cachedProjection = CSF_AT(uintptr_t, entry, 0x0C);
            CSF_AT(uint32_t, entry, 0x08) = instances->field_0034;
            if (CSF_AT(int, entry, 0x04) != -1)
            {
                GfxObjectInstances_TouchCachedEntry(
                    instances, nullptr, reinterpret_cast<int>(entry));
            }
        }
    }

    C_00867358_ClearFields(
        reinterpret_cast<C_00867358*>(cachedProjection), nullptr);

    auto* object = reinterpret_cast<uint8_t*>(objectAddress);
    CSF_AT(uintptr_t, context, 0x04) = reinterpret_cast<uintptr_t>(object + 0x20);
    std::memset(context + 0x30, 0, 0x24);
    CSF_AT(uintptr_t, context, 0x00) = owner;

    direction[0] = CSF_AT(float, object, 0x40);
    direction[1] = CSF_AT(float, object, 0x44);
    direction[2] = CSF_AT(float, object, 0x48);
    const int transformAddress = CSF_AT(int, object, 0x04) + 16;
    unitVector[0] = 0.0f;
    unitVector[1] = 0.0f;
    unitVector[2] = 1.0f;
    RwV3dTransformVector(
        reinterpret_cast<int>(direction),
        reinterpret_cast<int>(unitVector),
        transformAddress);

    CSF_AT(float, context, 0x14) = radius;
    CSF_AT(float, context, 0x1C) = radius * length;
    CSF_AT(float, context, 0x18) = length;
    CSF_AT(int, context, 0x20) = flags;
    CSF_AT(float, context, 0x24) = scale;

    cachedProjection = 0;
    const int activeIndex = *reinterpret_cast<int*>(entryIndices->field_0000);
    if (activeIndex != -1)
    {
        auto* entry = reinterpret_cast<uint8_t*>(instances->field_0008)
                    + static_cast<uint32_t>(activeIndex) * 16;
        if (CSF_AT(uintptr_t, entry, 0x00) == owner)
        {
            cachedProjection = CSF_AT(uintptr_t, entry, 0x0C);
            CSF_AT(uint32_t, entry, 0x08) = instances->field_0034;
            if (CSF_AT(int, entry, 0x04) != -1)
            {
                GfxObjectInstances_TouchCachedEntry(
                    instances, nullptr, reinterpret_cast<int>(entry));
            }
        }
    }
    CSF_AT(uintptr_t, context, 0x28) = cachedProjection;

    CSF_AT(float, context, 0x30) = origin[0];
    CSF_AT(float, context, 0x34) = origin[1];
    CSF_AT(float, context, 0x38) = origin[2];

    const float originX = origin[0];
    const float originY = origin[1];
    const float originZ = origin[2];
    CSF_AT(float, context, 0x58) = originX;
    CSF_AT(float, context, 0x5C) = originY;
    CSF_AT(float, context, 0x60) = originZ;
    CSF_AT(uintptr_t, context, 0x2C) = reinterpret_cast<uintptr_t>(instances);

    scratch[0] = originX - direction[0] * radius * scale;
    scratch[1] = originY - direction[1] * radius * scale;
    scratch[2] = originZ - direction[2] * radius * scale;
    CSF_AT(float, context, 0x48) = scratch[0];
    CSF_AT(float, context, 0x50) = scratch[2];
    CSF_AT(float, context, 0x4C) =
        static_cast<float>(scratch[1] + g_ProjectionHeightOffset); // 40.0

    scratch[0] = static_cast<float>(originX - direction[0] * g_ProjectionRayDistance);
    scratch[1] = static_cast<float>(originY - direction[1] * g_ProjectionRayDistance);
    scratch[2] = static_cast<float>(originZ - direction[2] * g_ProjectionRayDistance);
    CSF_AT(float, context, 0x44) = scratch[2];
    CSF_AT(float, context, 0x3C) = scratch[0];
    CSF_AT(float, context, 0x40) = scratch[1]; // Ray distance: 100000000.0

    auto* basis = reinterpret_cast<float*>(CSF_AT(uintptr_t, context, 0x04));
    scratch[0] = basis[12] - origin[0];
    scratch[1] = basis[13] - origin[1];
    scratch[2] = basis[14] - origin[2];
    RwV3dLength(scratch);

    CSF_AT(int, intersection, 0x18) = 4;
    scratch[0] = radius + origin[0];
    CSF_AT(float, intersection, 0x00) = scratch[0];
    scratch[1] = radius + origin[1];
    CSF_AT(float, intersection, 0x04) = scratch[1];
    scratch[2] = radius + origin[2];
    CSF_AT(float, intersection, 0x08) = scratch[2];

    scratch[0] = origin[0] - radius;
    CSF_AT(float, intersection, 0x0C) = scratch[0];
    scratch[1] = origin[1] - radius;
    CSF_AT(float, intersection, 0x10) = scratch[1];
    scratch[2] = origin[2] - radius;
    CSF_AT(float, intersection, 0x14) = scratch[2];

    const float endX = direction[0] * length + origin[0];
    if (endX >= CSF_AT(float, intersection, 0x00))
        CSF_AT(float, intersection, 0x00) = endX;

    const float endY = direction[1] * length + origin[1];
    if (endY >= CSF_AT(float, intersection, 0x04))
        CSF_AT(float, intersection, 0x04) = endY;

    const float endZ = direction[2] * length + origin[2];
    if (endZ >= CSF_AT(float, intersection, 0x08))
        CSF_AT(float, intersection, 0x08) = endZ;

    CSF_AT(float, intersection, 0x0C) =
        endX >= CSF_AT(float, intersection, 0x0C)
        ? CSF_AT(float, intersection, 0x0C) : endX;
    CSF_AT(float, intersection, 0x10) =
        endY >= CSF_AT(float, intersection, 0x10)
        ? CSF_AT(float, intersection, 0x10) : endY;
    CSF_AT(float, intersection, 0x14) =
        endZ >= CSF_AT(float, intersection, 0x14)
        ? CSF_AT(float, intersection, 0x14) : endZ;

    int controllerAddress = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    auto* controller = reinterpret_cast<void*>(controllerAddress);
    const int mainScene = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[68 / 4])(controller, nullptr);
    const int mainWorld = SceneLoader_GetWorldObjectCollection(
        reinterpret_cast<void*>(mainScene), nullptr);
    if (mainWorld)
    {
        RpCollisionWorldForAllIntersections(
            mainWorld, intersection, CSF_ADDR_FN(0x007014B0), context);
    }

    controllerAddress = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    controller = reinterpret_cast<void*>(controllerAddress);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[136 / 4])(controller, nullptr);
    const uint32_t sceneCount = static_cast<uint32_t>(result);
    uint32_t sceneIndex = 0;
    if (sceneCount != 0)
    {
        do
        {
            result = reinterpret_cast<int(__fastcall*)(void*, void*, unsigned int)>(
                CSF_AT(void**, controller, 0)[144 / 4])(
                    controller, nullptr, sceneIndex);
            if (result)
            {
                const int world = SceneLoader_GetWorldObjectCollection(
                    reinterpret_cast<void*>(result), nullptr);
                result = RpCollisionWorldForAllIntersections(
                    world, intersection, CSF_ADDR_FN(0x007014B0), context);
            }
            ++sceneIndex;
        }
        while (sceneIndex < sceneCount);
    }
    return reinterpret_cast<C_00863ac0*>(result);
}

HOOK(0x00701850, GfxObjectInstances_UpdateProjection, "auto");
