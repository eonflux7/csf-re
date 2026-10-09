#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObject_UpdateRelatedState, 0x00425710, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833ee8_PreparePlacement, 0x00493250, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008342e8_GetPlacementData, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_008342e8_SelectPlacement, 0x0049F220, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833ee8_RepositionAndResetState(
    C_00833ee8* self, void* /*edx*/)
{
    auto* source = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(self->pad_01b4)));

    float position[3];
    std::memcpy(
        position,
        reinterpret_cast<const void*>(GetObjectPositionVector(self, nullptr)),
        sizeof(position));

    C_008342e8_SelectPlacement(
        source, nullptr, static_cast<int>(self->field_01c0));
    C_00833ee8_PreparePlacement(self, nullptr);

    uint32_t placementData[6] = {};
    C_008342e8_GetPlacementData(
        source, nullptr, static_cast<int>(self->field_01c0), placementData);

    void* component = reinterpret_cast<void*>(self->vGetField016c());
    void* vectorOwner = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, component, 0)[0x70 / 4])(component, nullptr));

    uint32_t localVector[3];
    std::memcpy(
        localVector,
        reinterpret_cast<uint8_t*>(vectorOwner) + 0x18,
        sizeof(localVector));

    component = reinterpret_cast<void*>(self->vGetField016c());
    void* transform = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, component, 0)[0x18 / 4])(component, nullptr));

    float transformedVector[3] = {};
    reinterpret_cast<void(__fastcall*)(void*, void*, float*, uint32_t*)>(
        CSF_AT(void**, transform, 0)[0xA4 / 4])(
            transform, nullptr, transformedVector, localVector);

    if (source->field_0050 == 7)
    {
        SceneLoader_ConfigureFocusEntry(
            self, nullptr, reinterpret_cast<int*>(position));
    }
    else
    {
        float adjustedPosition[3];
        adjustedPosition[0] =
            position[0] - (transformedVector[0] + transformedVector[0]);
        adjustedPosition[1] =
            position[1] - (transformedVector[1] + transformedVector[1]);
        adjustedPosition[2] =
            position[2] - (transformedVector[2] + transformedVector[2]);

        SceneLoader_ConfigureFocusEntry(
            self, nullptr, reinterpret_cast<int*>(adjustedPosition));
    }

    const double sourceScalar = ComputeScalarForResolvedObject(source, nullptr);
    float scalarOffset;
    std::memcpy(&scalarOffset, &self->field_01ac, sizeof(scalarOffset));
    const float scalar = static_cast<float>(sourceScalar + scalarOffset);

    int payload;
    std::memcpy(&payload, &scalar, sizeof(payload));
    DispatchSceneObjectWithPayload(self, nullptr, payload);

    self->field_0400 = 1;

    void* controller = reinterpret_cast<void*>(source->field_0044);
    if (controller)
    {
        void* entry = reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, controller, 0)[0xC8 / 4])(
                    controller, nullptr, 0));
        if (entry)
            CSF_AT(uint8_t, entry, 0x16) = 0;
    }

    self->field_02e0 &= ~0x2000u;
    self->field_02e4 &= 0xFFFFDFFF;
    self->field_01ac = 0;
    self->field_03a5 = 0;
    self->vSlot6C(1);
    self->vSlot94();

    if (source->field_0050 == 7 &&
        static_cast<int32_t>(self->field_03c8) >= 0)
    {
        void* relatedObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(&self->field_03c0)));
        if (relatedObject)
        {
            void* relatedState = reinterpret_cast<void*>(
                CSF_AT(uint32_t, relatedObject, 0x30));
            if (relatedState)
                SceneObject_UpdateRelatedState(relatedState, nullptr);
        }
    }

    self->field_03c4 = -1;
    self->field_03c8 = -1;
    return -1;
}

HOOK(0x004993E0, C_00833ee8_RepositionAndResetState, "auto");
