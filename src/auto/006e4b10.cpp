#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RpWorldAddCamera, 0x0058DD90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x00628E40, (int camera));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50, (int camera));
CSF_FUNC(int, __cdecl, RwCameraClear, 0x00628F60, (int camera, int color, int flags));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_00860bb8_GetLoadedScene, 0x006CBAC0, (C_00860bb8* self, void*));

CSF_FUNC(char, __fastcall, S_006e48a0_RenderObject, 0x006E4680,
         (void* self, void* edx, C_0085e7b8* object, void* provider, int temporaryMode));
CSF_FUNC(int, __fastcall, S_006e48a0_ConfigureCamera, 0x006E48A0,
         (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(float*, __fastcall, S_006e48a0_UpdateProjectionCenter, 0x006E4990,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObjectInstances_RegisterInstance, 0x007012E0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxObjectInstances_FindInstance, 0x00701470,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, GfxObjectInstances_UpdateProjection, 0x00701850,
         (void* self, void* edx, int a2, float* a3, float a4, float a5, int a6, float a7, float a8));
CSF_FUNC(bool, __fastcall, GfxObjectInstances_CheckProjectionChange, 0x00701DC0,
         (void* self, void* edx, int* a2));

CSF_GLOBAL(float, g_InstanceRadiusLimit, 0x00861B78);
CSF_GLOBAL(float, g_InstanceMovementThreshold, 0x00861B7C);
CSF_GLOBAL(float, g_InstanceProjectionThreshold, 0x00861B8C);
CSF_GLOBAL(float, g_InstanceProjectionHeight, 0x00861B90);

static float GfxObjectInstances_FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static void __fastcall S_006e48a0_RenderProjection(
    S_006e48a0* self, void* /*edx*/, C_0085e7b8* object,
    uint32_t* context, int mode, float* points)
{
    if (!(self->field_001c < g_InstanceRadiusLimit) ||
        !GetCachedObjectProvider(object, nullptr))
        return;

    void* instance = nullptr;
    bool projectionChanged = false;

    if (mode != 3)
    {
        if (self->field_0020 == uint32_t(-1))
        {
            self->field_0020 = GfxObjectInstances_RegisterInstance(
                context + 17, nullptr, reinterpret_cast<int>(self));
            projectionChanged = true;
        }

        instance = reinterpret_cast<void*>(GfxObjectInstances_FindInstance(
            context + 17, nullptr, self->field_0020, reinterpret_cast<int>(self)));

        if (!instance)
        {
            const int index = GfxObjectInstances_RegisterInstance(
                context + 17, nullptr, reinterpret_cast<int>(self));
            self->field_0020 = index;
            instance = reinterpret_cast<void*>(GfxObjectInstances_FindInstance(
                context + 17, nullptr, index, reinterpret_cast<int>(self)));
            projectionChanged = true;
            if (!instance)
            {
                self->field_0020 = uint32_t(-1);
                return;
            }
        }
        else if (!projectionChanged)
        {
            projectionChanged = GfxObjectInstances_CheckProjectionChange(
                instance, nullptr, reinterpret_cast<int*>(context + 33));
        }
    }

    const int global = GetGlobalInt();
    const int controller = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(global), nullptr);
    const int scene = C_00860bb8_GetLoadedScene(
        reinterpret_cast<C_00860bb8*>(controller), nullptr);

    const int globalState = GetGlobalValue_6C5760();
    void* provider = GetCachedObjectProvider(
        CSF_AT(void*, reinterpret_cast<void*>(globalState), 0xF0), nullptr);

    const int accessorGlobal = GetGlobalInt();
    GlobalInt_ResolveAccessor(reinterpret_cast<void*>(accessorGlobal), nullptr);
    SceneLoader_NoOp();

    const auto position = reinterpret_cast<uint32_t*(
        __fastcall*)(void*, void*)>(object->vtbl[0x28 / 4])(object, nullptr);

    alignas(8) uint8_t positionBytes[0x1C] = {};
    const uint32_t positionX = position[0];
    CSF_AT(uint32_t, positionBytes, 0x00) = positionX;
    const float deltaX =
        GfxObjectInstances_FloatFromBits(self->field_0004) -
        CSF_AT(float, positionBytes, 0x00);

    const uint32_t positionY = position[1];
    CSF_AT(uint32_t, positionBytes, 0x04) = positionY;
    const float deltaY = self->field_0008 - CSF_AT(float, positionBytes, 0x04);

    const uint32_t positionZ = position[2];
    CSF_AT(uint32_t, positionBytes, 0x08) = positionZ;
    const float deltaZ =
        GfxObjectInstances_FloatFromBits(self->field_000c) -
        CSF_AT(float, positionBytes, 0x08);

    const float distance = std::sqrt(
        deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX);
    bool moved = false;
    if (distance > g_InstanceMovementThreshold)
    {
        self->field_0004 = positionX;
        std::memcpy(&self->field_0008, &positionY, sizeof(positionY));
        self->field_000c = positionZ;
        moved = true;
        if (mode == 3)
            S_006e48a0_UpdateProjectionCenter(self, nullptr);
    }

    // 0x006E48A0 is a direct callee, not a vtable.
    S_006e48a0_ConfigureCamera(
        self, nullptr, context[49], reinterpret_cast<int>(context), self->field_001c);

    auto camera = reinterpret_cast<uint32_t*>(context[49]);
    RpWorldAddCamera(scene, reinterpret_cast<int>(camera));
    camera[24] = context[1];
    camera[25] = 0;

    alignas(8) uint8_t clearColorBytes[0x08] = {};
    const bool cleared = RwCameraClear(
        reinterpret_cast<int>(camera), reinterpret_cast<int>(clearColorBytes), 1) != 0;

    const uint32_t raster = context[2];
    const int frame = camera[1];
    camera[24] = raster;
    camera[25] = 0;
    RwFrameUpdateObjects(frame);

    if (cleared && RwCameraBeginUpdate(reinterpret_cast<int>(camera)))
    {
        const int temporaryMode = mode == 2;
        S_006e48a0_RenderObject(self, nullptr, object, provider, temporaryMode);
        auto secondaryObject = reinterpret_cast<C_0085e7b8*>(self->field_0024);
        if (secondaryObject)
            S_006e48a0_RenderObject(
                self, nullptr, secondaryObject, provider, temporaryMode);
        RwCameraEndUpdate(reinterpret_cast<int>(camera));
    }

    RpWorldRemoveCamera(scene, reinterpret_cast<int>(camera));

    if (!moved && !projectionChanged && mode != 3)
        return;

    if (mode == 3)
    {
        const float radius = self->field_001c;
        {
            const float x = GfxObjectInstances_FloatFromBits(self->field_0010) - radius;
            const float y = GfxObjectInstances_FloatFromBits(self->field_0014) +
                            g_InstanceProjectionHeight;
            const float z = GfxObjectInstances_FloatFromBits(self->field_0018) - radius;
            points[12] = x;
            points[13] = y;
            points[14] = z;
        }
        {
            const float x = GfxObjectInstances_FloatFromBits(self->field_0010) - radius;
            const float y = GfxObjectInstances_FloatFromBits(self->field_0014) +
                            g_InstanceProjectionHeight;
            const float z = radius + GfxObjectInstances_FloatFromBits(self->field_0018);
            points[15] = x;
            points[16] = y;
            points[17] = z;
        }
        {
            const float x = radius + GfxObjectInstances_FloatFromBits(self->field_0010);
            const float y = GfxObjectInstances_FloatFromBits(self->field_0014) +
                            g_InstanceProjectionHeight;
            const float z = GfxObjectInstances_FloatFromBits(self->field_0018) - radius;
            points[18] = x;
            points[19] = y;
            points[20] = z;
        }
        {
            const float x = radius + GfxObjectInstances_FloatFromBits(self->field_0010);
            const float y = GfxObjectInstances_FloatFromBits(self->field_0014) +
                            g_InstanceProjectionHeight;
            const float z = radius + GfxObjectInstances_FloatFromBits(self->field_0018);
            points[21] = x;
            points[22] = y;
            points[23] = z;
        }

        RwV3dTransformPoints(
            reinterpret_cast<int>(points), reinterpret_cast<int>(points + 12),
            4, reinterpret_cast<int>(camera + 8));
        return;
    }

    const float blend = self->field_001c > g_InstanceProjectionThreshold
        ? 0.75f : 0.25f;

    // The supplied binding types this pointer-valued stack argument as float.
    const float projectionDataBits = GfxObjectInstances_FloatFromBits(
        reinterpret_cast<uint32_t>(context + 33));
    GfxObjectInstances_UpdateProjection(
        instance, nullptr, reinterpret_cast<int>(camera),
        reinterpret_cast<float*>(positionBytes), self->field_001c,
        g_InstanceProjectionThreshold, 1123024896, projectionDataBits, blend);
}

HOOK(0x006E4B10, S_006e48a0_RenderProjection, "auto");
