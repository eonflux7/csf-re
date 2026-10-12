#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, QueryLeafState, 0x006B6D70, (void* self, void* edx, float* a2, char* a3));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_LeafTransitionDistanceScale, 0x00861BFC);

static char __fastcall GfxObjectInstances_UpdateLeafState(
    float* self, void* /*edx*/, float* position)
{
    if (position[0] != CSF_AT(float, self, 0x04) ||
        position[1] != CSF_AT(float, self, 0x08) ||
        position[2] != CSF_AT(float, self, 0x0C))
    {
        const float deltaX = position[0] - CSF_AT(float, self, 0x04);
        char targetState = CSF_AT(char, self, 0x02);
        const float deltaY = position[1] - CSF_AT(float, self, 0x08);
        const float deltaZ = position[2] - CSF_AT(float, self, 0x0C);

        if (CSF_AT(std::uint8_t, self, 0x00) != 0)
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* scene = reinterpret_cast<void*>(
                SceneLoader_GetSceneController(global, nullptr));

            void* leafStateProvider =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, scene, 0)[0x44 / 4])(scene, nullptr);

            QueryLeafState(leafStateProvider, nullptr, position, &targetState);
        }

        float blend = std::sqrt(
            deltaZ * deltaZ + deltaX * deltaX + deltaY * deltaY) *
            g_LeafTransitionDistanceScale;
        if (blend > g_TransitionOne)
            blend = g_TransitionOne;

        const int currentState = CSF_AT(std::uint8_t, self, 0x01);
        const int destinationState = static_cast<unsigned char>(targetState);
        const std::int64_t interpolatedState = static_cast<std::int64_t>(
            (g_TransitionOne - blend) * currentState + destinationState * blend);
        CSF_AT(std::uint8_t, self, 0x01) =
            static_cast<std::uint8_t>(interpolatedState);

        CSF_AT(float, self, 0x04) = position[0];
        CSF_AT(float, self, 0x08) = position[1];
        CSF_AT(float, self, 0x0C) = position[2];
    }

    return CSF_AT(char, self, 0x01);
}

HOOK(0x006E5370, GfxObjectInstances_UpdateLeafState, "auto");
