#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004b1050_HasCompatibleNearbyObject, 0x004B1050, (S_004b1050* self, void* , int linkedObject));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Retain the packet's address binding; the instruction actually calls this address.
[[maybe_unused]] static const auto kCompatibilityBinding = CSF_VTBL(0x004B1050);

static char __fastcall S_004b1050_CheckDistractionConditions(
    S_004b1050* self, void* /*edx*/, int linkedObject)
{
    void* sceneContext = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!sceneContext)
        return 0;

    const int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(linkedObject), nullptr);
    void* linkedOwner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    const float* position = GetObjectPositionVector(
        CSF_AT(void*, linkedOwner, 0x2C), nullptr);
    const float sourceX = position[0];
    const float sourceY = position[1];
    const float sourceZ = position[2];

    alignas(8) uint8_t segmentBytes[0x48];
    alignas(8) uint8_t queryBytes[0x10];
    float* segment = reinterpret_cast<float*>(segmentBytes);
    int* query = reinterpret_cast<int*>(queryBytes);

    char accepted = 0;
    uint32_t attempts = 0;
    while (!accepted)
    {
        position = GetObjectPositionVector(sceneContext, nullptr);
        float destinationX = position[0];
        const float destinationZ = position[2];
        const float destinationY = position[1];

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        destinationX += static_cast<float>(GetStateValue(state, nullptr, 400) - 200);
        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        const float randomizedZ =
            static_cast<float>(GetStateValue(state, nullptr, 400) - 200) + destinationZ;

        std::memcpy(&self->field_0014, &destinationX, sizeof(destinationX));
        CSF_AT(float, self, 0x18) = destinationY;
        CSF_AT(float, self, 0x1C) = randomizedZ;

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        void* collisionQuery =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr);

        const float raisedDestinationY = CSF_AT(float, self, 0x18) + g_MaxPercent;
        std::memcpy(&segment[3], &self->field_0014, sizeof(float));
        segment[4] = raisedDestinationY;
        segment[5] = CSF_AT(float, self, 0x1C);
        segment[0] = sourceX;
        segment[1] = sourceY + g_MaxPercent;
        segment[2] = sourceZ;

        query[0] = 2;
        const bool obstructed =
            ((bool (__fastcall*)(void*, void*, float*, int*, int))
                CSF_AT(void**, collisionQuery, 0)[0x24 / 4])(
                    collisionQuery, nullptr, segment, query, 3);

        if (obstructed)
            self->field_0020 = 5;
        else if (S_004b1050_HasCompatibleNearbyObject(self, nullptr, linkedObject))
            self->field_0020 = 6;
        else
            accepted = 1;

        if (++attempts >= 0xA)
            return accepted;
    }
    return accepted;
}

HOOK(0x004B68D0, S_004b1050_CheckDistractionConditions, "auto");
