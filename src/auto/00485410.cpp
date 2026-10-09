#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_ResetTargetController, 0x00409440, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_GetTargetTransform, 0x00409470, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_GetFacingEndpoints, 0x004094A0, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, CharActionFsm_SetControllerPosition, 0x004096D0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_GetProviderObject, 0x004F4F30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_SetProviderFacing, 0x004F5040, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetVectorAngleRadians, 0x006AB930, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_RadiansToDegrees, 0x00833434);
CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharActionFsm_InitializeTargetFacing(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x8);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, owner, 0)[0x6C / 4])(owner, nullptr, 0);

    int manager = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(manager), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    if (!provider)
        return 0;

    uint32_t requestOrStart[3];
    requestOrStart[0] = reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448));
    requestOrStart[1] = 0xFFFFFFFF;
    requestOrStart[2] = 0xFFFFFFFF;
    ObjectProvider_SetPendingRequest(
        provider, nullptr, reinterpret_cast<int>(requestOrStart));

    owner = CSF_AT(void*, self, 0x8);
    if (CSF_AT(int, owner, 0x2BC) < 0)
        return reinterpret_cast<int>(static_cast<uint8_t*>(owner) + 0x2B0);

    int target = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(owner) + 0x2B4));
    if (!target)
        return target;

    void* targetObject = CSF_AT(void*, reinterpret_cast<void*>(target), 0x4C);
    // The original forwards an unspecified saved register to this slot.
    void* controller = reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, targetObject, 0)[0x3C / 4])(targetObject, nullptr, 0);

    int position = GetObjectPositionVector(CSF_AT(void*, self, 0x8), nullptr);
    CharActionFsm_SetControllerPosition(
        controller, nullptr, reinterpret_cast<float*>(position));

    requestOrStart[0] = 0;
    requestOrStart[1] = 0;
    requestOrStart[2] = 0;
    uint32_t endpointAndAngle[4] = {};
    CharActionFsm_GetFacingEndpoints(
        controller, nullptr, requestOrStart, endpointAndAngle);

    uint32_t transformAndDirection[6] = {};
    CharActionFsm_GetTargetTransform(controller, nullptr, transformAndDirection);
    CharActionFsm_GetProviderObject(provider, nullptr);
    ObjectProvider_SetUpdateMode(provider, nullptr, 6);

    CSF_AT(float, transformAndDirection, 0xC) =
        CSF_AT(float, endpointAndAngle, 0) - CSF_AT(float, requestOrStart, 0);
    CSF_AT(float, transformAndDirection, 0x10) =
        CSF_AT(float, endpointAndAngle, 4) - CSF_AT(float, requestOrStart, 4);
    CSF_AT(float, transformAndDirection, 0x14) =
        CSF_AT(float, endpointAndAngle, 8) - CSF_AT(float, requestOrStart, 8);

    CSF_AT(float, endpointAndAngle, 0xC) = static_cast<float>(
        GetVectorAngleRadians(
            reinterpret_cast<uint8_t*>(transformAndDirection) + 0xC, nullptr) *
        g_RadiansToDegrees - g_FacingAngleOffset);

    int firstSettings = GetGlobalValue_8BC1E0();
    int secondSettings = GetGlobalValue_8BC1E0();
    int facingParameter = CSF_AT(int, reinterpret_cast<void*>(firstSettings), 0x2278);
    int rotationParameter = CSF_AT(int, reinterpret_cast<void*>(secondSettings), 0x227C);
    CharActionFsm_SetProviderFacing(
        provider, nullptr, 0, CSF_AT(int, endpointAndAngle, 0xC),
        rotationParameter, facingParameter);

    return CharActionFsm_ResetTargetController(controller, nullptr);
}

HOOK(0x00485410, CharActionFsm_InitializeTargetFacing, "auto");
