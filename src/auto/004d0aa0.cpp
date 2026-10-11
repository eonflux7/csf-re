#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_0083b770_CanUseSceneObject, 0x004D09B0, (void* self, void* edx, float a2, int a3));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static uint32_t* __fastcall C_0083b770_FindNearbyActiveSceneEntry(
    C_0083b770* self, void* /*edx*/)
{
    void* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0014), nullptr);
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const uint32_t entryCount = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    const float distanceMargin = CSF_AT(float, settings, 0x1844);

    alignas(8) uint8_t positionMatrixBytes[0x3C];
    alignas(8) uint8_t boundsMatrixBytes[0x3C];
    alignas(8) uint8_t displacementBytes[0xC];
    alignas(8) uint8_t sphereBytes[0x28];

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        uint32_t* entry = CSF_AT(uint32_t**, g_pCurrentScene, 0x20)[index];
        if (!entry)
            continue;

        if (static_cast<uint8_t>(
                GameConfig_IsResolvedEntryActive(CSF_AT(int, entry, 0x50))) == 0)
            continue;

        void* task = CSF_AT(void*, entry, 0x30);
        if (!task)
            continue;

        if (((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, task, 0)[0xB8 / 4])(task, nullptr))
            continue;

        void* dispatcher = CSF_AT(void*, entry, 0x4C);
        if (!dispatcher)
            continue;

        void* object = C_0082dd78_DispatchReturnedObject(dispatcher, nullptr);
        if (!object)
            continue;

        void* geometry = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr);
        if (!geometry)
            continue;

        float* sphere = reinterpret_cast<float*>(sphereBytes);
        sphere[0] = 0.0f;
        sphere[1] = 0.0f;
        sphere[2] = 0.0f;
        sphere[3] = 0.0f;

        // The sphere output is pushed before the matrix getter, but belongs
        // to GetBoundingSphere, which consumes it after the getter returns.
        float* boundsMatrix = ((float*(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, geometry, 0)[0x1C / 4])(
                geometry, nullptr, reinterpret_cast<float*>(boundsMatrixBytes));
        GfxMatrix_GetBoundingSphere(boundsMatrix, nullptr, sphere);

        float* positionMatrix = ((float*(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, geometry, 0)[0x1C / 4])(
                geometry, nullptr, reinterpret_cast<float*>(positionMatrixBytes));
        float* displacement = BuildFacingVectorBetween(
            position, nullptr, reinterpret_cast<float*>(displacementBytes),
            positionMatrix);
        const double distance = Vector_GetLength(displacement, nullptr);

        if (distance - distanceMargin < sphere[3])
        {
            int requestedType = -1;
            if (CSF_AT(uint32_t, entry, 0x50) == 4)
                requestedType = 20;

            // This callee's float parameter carries the object's address bits.
            const uint32_t objectAddress = reinterpret_cast<uint32_t>(object);
            float objectArgument;
            std::memcpy(&objectArgument, &objectAddress, sizeof(objectArgument));
            if (C_0083b770_CanUseSceneObject(
                    self, nullptr, objectArgument, requestedType))
                return entry;
        }
    }

    return nullptr;
}

HOOK(0x004D0AA0, C_0083b770_FindNearbyActiveSceneEntry, "auto");
