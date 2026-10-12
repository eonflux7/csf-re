#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeHitInfo, 0x0040ED90, (void* self, void* edx));
CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyChildPosition, 0x00450210, (CGuiControl* self, void* , int referenceObject, float maximumDistance, uint32_t* excludedFirst, uint32_t* excludedSecond, uint32_t* outputPosition));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall S_004af450_FindNearbyPosition(
    S_004af450* self, void* /*edx*/, float* segment, uint32_t* outputPosition)
{
    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalObject, nullptr));

    void* queryService =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr);

    void* hitObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int, float*, int)>(
            CSF_AT(void**, queryService, 0)[0x38 / 4])(
                queryService, nullptr, 0, segment, 3);

    if (hitObject)
    {
        alignas(8) uint8_t hitInfoBytes[0x68];
        CharActionFsm_InitializeHitInfo(hitInfoBytes, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, hitObject, 0)[0xC / 4])(
                hitObject, nullptr, hitInfoBytes);

        uint32_t x = CSF_AT(uint32_t, hitInfoBytes, 0x4);
        uint32_t y = CSF_AT(uint32_t, hitInfoBytes, 0x8);
        outputPosition[0] = x;
        uint32_t z = CSF_AT(uint32_t, hitInfoBytes, 0xC);
        outputPosition[1] = y;
        outputPosition[2] = z;
        return 1;
    }

    alignas(8) uint8_t directionBytes[0xC];
    float* direction = reinterpret_cast<float*>(directionBytes);
    direction[0] = segment[3] - segment[0];
    direction[1] = segment[4] - segment[1];
    direction[2] = segment[5] - segment[2];

    float maximumDistance = std::sqrt(
        direction[2] * direction[2] +
        direction[1] * direction[1] +
        direction[0] * direction[0]);

    alignas(8) uint8_t contextBytes[0x18];
    SpatialQuery_BuildContext(
        reinterpret_cast<C_00833b88*>(contextBytes), nullptr,
        reinterpret_cast<uint32_t*>(segment),
        reinterpret_cast<uint32_t*>(directionBytes));

    int linkedObject = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<uint8_t*>(self) + 0x8, nullptr);
    uint8_t* linkedOwner = linkedObject
        ? reinterpret_cast<uint8_t*>(linkedObject) - 4
        : nullptr;
    uint32_t* excludedFirst = CSF_AT(uint32_t*, linkedOwner, 0x2C);

    return CGuiControl_FindNearbyChildPosition(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(contextBytes), maximumDistance,
        excludedFirst, nullptr, outputPosition);
}

HOOK(0x004AF450, S_004af450_FindNearbyPosition, "auto");
