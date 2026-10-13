#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyChildPosition, 0x00450210, (CGuiControl* self, void* , int referenceObject, float maximumDistance, uint32_t* excludedFirst, uint32_t* excludedSecond, uint32_t* outputPosition));
CSF_FUNC(char, __fastcall, CGuiControl_HasListenerIntersectionBefore, 0x00452F20, (CGuiControl* self, void* , int rayAddress, float maximumDistance));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __stdcall SpatialQuery_IsSegmentUnobstructed(
    float* endpoints, uint32_t* excludedFirst, uint32_t* excludedSecond)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalState, nullptr));

    void* query = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr);

    int queryMask = 4;
    bool obstructed =
        reinterpret_cast<bool (__fastcall*)(void*, void*, float*, int*, int)>(
            CSF_AT(void**, query, 0)[0x24 / 4])(
                query, nullptr, endpoints, &queryMask, 3);

    if (!obstructed)
    {
        alignas(8) uint8_t contextBytes[0x18];
        C_00833b88& context =
            *reinterpret_cast<C_00833b88*>(contextBytes);

        alignas(8) uint8_t directionBytes[0xC];
        float* direction = reinterpret_cast<float*>(directionBytes);
        direction[0] = endpoints[3] - endpoints[0];
        direction[1] = endpoints[4] - endpoints[1];
        direction[2] = endpoints[5] - endpoints[2];

        float maximumDistance = std::sqrt(
            direction[2] * direction[2] +
            direction[1] * direction[1] +
            direction[0] * direction[0]);

        SpatialQuery_BuildContext(
            &context, nullptr,
            reinterpret_cast<uint32_t*>(endpoints),
            reinterpret_cast<uint32_t*>(direction));

        alignas(8) uint8_t outputBytes[0xC];
        uint32_t* outputPosition = reinterpret_cast<uint32_t*>(outputBytes);
        outputPosition[0] = 0;
        outputPosition[1] = 0;
        outputPosition[2] = 0;

        obstructed = CGuiControl_FindNearbyChildPosition(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(&context), maximumDistance,
            excludedFirst, excludedSecond, outputPosition) != 0;

        if (!obstructed)
        {
            obstructed = CGuiControl_HasListenerIntersectionBefore(
                reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
                reinterpret_cast<int>(&context), maximumDistance) != 0;
        }
    }

    return !obstructed;
}

HOOK(0x0044DF20, SpatialQuery_IsSegmentUnobstructed, "auto");
