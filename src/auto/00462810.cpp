#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, ConstructCommandBlock, 0x00412AB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, ClampIntegerToUnorderedBounds, 0x0045BB60, (int* value, int* firstBound, int* secondBound));
CSF_FUNC(int, __cdecl, IsObjectKindEnabled, 0x00755810, (unsigned int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Controller object (scene handle +0x38) vtable shapes, invoked through the vtable slot (byte offset / 4).
using ControllerQueryFn = int(__fastcall*)(void* self, void* edx);
using ControllerSetValueFn = int(__fastcall*)(void* self, void* edx, int value);
using ControllerCommandFn = int(__fastcall*)(void* self, void* edx, void* commandBlock);

static int __stdcall ApplyValueCommandToSceneHandle(int /*a1*/, int /*a2*/, void* commandRecord)
{
    // commandRecord -> header; header+4 -> argument block; block+12 = signed flag; block+4 = handle value.
    void* messageHeader = CSF_AT(void*, commandRecord, 0);
    void* argumentBlock = CSF_AT(void*, messageHeader, 4);
    int argumentFlag = CSF_AT(int, argumentBlock, 12);
    int handleValue = static_cast<int>(reinterpret_cast<uintptr_t>(argumentBlock)) + 4;

    if (argumentFlag < 0) {
        return handleValue;
    }

    const int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);
    if (resolvedHandle == 0) {
        return resolvedHandle;
    }
    void* sceneObject = reinterpret_cast<void*>(resolvedHandle);

    const int objectKindPredicate = IsObjectKindEnabled(CSF_AT(uint32_t, sceneObject, 0x50));
    if (static_cast<uint8_t>(objectKindPredicate) == 0) {
        return objectKindPredicate;
    }

    void* controllerObject = CSF_AT(void*, sceneObject, 0x38);
    if (controllerObject == nullptr) {
        return objectKindPredicate;
    }

    // Requested value: float at commandRecord->[4]->[4] + 4, truncated toward zero to a 64-bit
    // integer, then the low 32 bits are kept (as the original's __ftol2 result in eax).
    void* payloadBlock = CSF_AT(void*, commandRecord, 4);
    void* payloadValueHolder = CSF_AT(void*, payloadBlock, 4);
    const float requestedValue = CSF_AT(float, payloadValueHolder, 4);
    const int64_t truncatedRequest = static_cast<int64_t>(requestedValue);
    int clampedTarget = static_cast<int32_t>(static_cast<uint32_t>(truncatedRequest & 0xFFFFFFFFLL));

    if (clampedTarget > 0) {
        int upperBound = reinterpret_cast<ControllerQueryFn>(CSF_AT(void**, controllerObject, 0)[33])(controllerObject, nullptr);
        int lowerBound = 0;
        ClampIntegerToUnorderedBounds(&clampedTarget, &lowerBound, &upperBound);
        void* controllerNow = CSF_AT(void*, sceneObject, 0x38);
        return reinterpret_cast<ControllerSetValueFn>(CSF_AT(void**, controllerNow, 0)[41])(controllerNow, nullptr, clampedTarget);
    }

    int currentValue = reinterpret_cast<ControllerQueryFn>(CSF_AT(void**, controllerObject, 0)[40])(controllerObject, nullptr);
    int deltaToCurrent = clampedTarget - currentValue;

    uint8_t commandBlock[52];
    ConstructCommandBlock(commandBlock, nullptr);

    void* controllerNow = CSF_AT(void*, sceneObject, 0x38);
    const uint32_t deltaBits = static_cast<uint32_t>(deltaToCurrent);
    const int absoluteDelta = static_cast<int>(deltaToCurrent < 0 ? 0u - deltaBits : deltaBits);
    *reinterpret_cast<int32_t*>(commandBlock + 16) = absoluteDelta;
    *reinterpret_cast<int32_t*>(commandBlock + 12) = -1;
    *reinterpret_cast<int32_t*>(commandBlock + 20) = 0;
    return reinterpret_cast<ControllerCommandFn>(CSF_AT(void**, controllerNow, 0)[35])(controllerNow, nullptr, commandBlock);
}

HOOK(0x00462810, ApplyValueCommandToSceneHandle, "auto");
