#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, _rpAtomicResyncInterpolatedSphere, 0x0058EB40, (int a1));
CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, SceneObject_GetWorldBounds, 0x006C0570, (uint32_t* a1, int arg4, int a7));
CSF_FUNC(double, __cdecl, GfxSkin_ComputeDistanceToWorldBounds, 0x006C5110, (float a1, float a2, float a3, float a4));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __cdecl GfxSkin_GetMaximumDistanceToObject(
    void* atomic, void* referenceObject)
{
    if (!atomic || !referenceObject)
        return g_fZero;

    void* transform =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, referenceObject, 0)[0x10 / 4])(
                referenceObject, nullptr);

    float* position =
        ((float* (__fastcall*)(void*, void*))
            CSF_AT(void**, transform, 0)[0x10 / 4])(
                transform, nullptr);

    alignas(8) uint8_t boundsBytes[0x24];
    alignas(8) uint8_t sphereCenterBytes[0xC];

    const float positionX = position[0];
    const float positionY = position[1];
    CSF_AT(float, boundsBytes, 0x18) = positionX;
    CSF_AT(float, boundsBytes, 0x1C) = positionY;
    CSF_AT(float, boundsBytes, 0x20) = position[2];

    const int atomicAddress = reinterpret_cast<int>(atomic);
    float* sphere = reinterpret_cast<float*>(
        RpAtomicGetWorldBoundingSphere(atomicAddress));

    CSF_AT(float, sphereCenterBytes, 0x0) = sphere[0];
    CSF_AT(float, sphereCenterBytes, 0x4) = sphere[1];
    CSF_AT(float, sphereCenterBytes, 0x8) = sphere[2];

    void* attachment = reinterpret_cast<void*>(
        SceneLoader_GetObjectAttachment(atomicAddress));

    if (attachment &&
        static_cast<uint8_t>(GetBit2OfField4(attachment, nullptr)))
    {
        SceneObject_GetWorldBounds(
            reinterpret_cast<uint32_t*>(boundsBytes), atomicAddress, 1);
        C_0085ec18_NullSub(reinterpret_cast<int>(sphereCenterBytes));

        // The supplied callee signature represents this pointer argument
        // as a float; preserve its argument bits rather than converting it.
        const uint32_t boundsAddress =
            reinterpret_cast<uint32_t>(boundsBytes);
        float boundsArgument;
        std::memcpy(&boundsArgument, &boundsAddress, sizeof(boundsArgument));

        return GfxSkin_ComputeDistanceToWorldBounds(
            positionX, positionY,
            CSF_AT(float, boundsBytes, 0x20), boundsArgument);
    }

    const bool needsResync = (CSF_AT(uint8_t, atomic, 0x50) & 2) != 0;
    const float dx = CSF_AT(float, sphereCenterBytes, 0x0)
                   - CSF_AT(float, boundsBytes, 0x18);
    const float dy = CSF_AT(float, sphereCenterBytes, 0x4)
                   - CSF_AT(float, boundsBytes, 0x1C);
    const float dz = CSF_AT(float, sphereCenterBytes, 0x8)
                   - CSF_AT(float, boundsBytes, 0x20);
    const float distance = std::sqrt(dz * dz + dy * dy + dx * dx);

    if (needsResync)
        _rpAtomicResyncInterpolatedSphere(atomicAddress);

    return distance + CSF_AT(float, atomic, 0x28);
}

HOOK(0x006C55B0, GfxSkin_GetMaximumDistanceToObject, "auto");
