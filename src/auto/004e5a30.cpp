#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyChildPosition, 0x00450210, (CGuiControl* self, void* edx, int referenceObject, float maximumDistance, uint32_t* excludedFirst, uint32_t* excludedSecond, uint32_t* outputPosition));
CSF_FUNC(char, __fastcall, CGuiControl_HasListenerIntersectionBefore, 0x00452F20, (CGuiControl* self, void* edx, int rayAddress, float maximumDistance));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall C_0083e9d0_IsSegmentClear(C_0083e9d0* self, void* /*edx*/, float* segment, uint32_t* outArg)
{
    void* globalObj = reinterpret_cast<void*>(GetGlobalInt());
    int sceneController = SceneLoader_GetSceneController(globalObj, nullptr);
    void* controller = reinterpret_cast<void*>(sceneController);

    void* queryObj = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[15])(controller, nullptr);

    int mode = 4;
    char result = ((char(__fastcall*)(void*, void*, float*, int*, int))CSF_AT(void**, queryObj, 0)[9])(queryObj, nullptr, segment, &mode, 3);

    if (!result) {
        alignas(8) uint8_t contextBytes[0x18];
        float delta[3];
        uint32_t outPos[3];

        delta[0] = segment[3] - segment[0];
        delta[1] = segment[4] - segment[1];
        delta[2] = segment[5] - segment[2];
        float length = (float)std::sqrt((double)(delta[2] * delta[2] + delta[1] * delta[1] + delta[0] * delta[0]));

        SpatialQuery_BuildContext(reinterpret_cast<C_00833b88*>(contextBytes), nullptr,
                                  reinterpret_cast<uint32_t*>(segment),
                                  reinterpret_cast<uint32_t*>(delta));

        outPos[0] = 0;
        outPos[1] = 0;
        outPos[2] = 0;

        result = CGuiControl_FindNearbyChildPosition(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(contextBytes), length, outArg,
            reinterpret_cast<uint32_t*>(self->field_0008), outPos);
        if (!result) {
            result = CGuiControl_HasListenerIntersectionBefore(
                reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
                reinterpret_cast<int>(contextBytes), length);
        }
    }
    return result == 0;
}

HOOK(0x004E5A30, C_0083e9d0_IsSegmentClear, "auto");
