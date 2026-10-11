#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, GfxHAnim_GetQuaternionAngle, 0x006AEA20,
         (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxHAnim_GetQuaternionAxis, 0x006AEA30,
         (void* self, void* edx, float* a2));
CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30,
         (int a1, float* a2, float a3, int a4));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_NegativeRadiansToDegrees, 0x00861F28);

static int __fastcall C_00861e98_RotateNode(
    C_00861e98* self, void* /*edx*/, void* hierarchy,
    int nodeIndex, float* axis, float angle)
{
    auto* frameData = CSF_AT(std::uint8_t*, hierarchy, 0x20);
    void* nodeInfo = CSF_AT(void*, hierarchy, 0x10);
    const int firstNode = CSF_AT(int, nodeInfo, 0x4);
    const int frameStride = CSF_AT(int, frameData, 0x24);
    auto* frame = frameData + 0x4C + frameStride * (nodeIndex - firstNode);
    const int quaternionAddress =
        static_cast<int>(reinterpret_cast<std::intptr_t>(frame + 0x8));

    auto* nodes = CSF_AT(void**, self, 0x4C);
    void* node = nodes[nodeIndex];
    if (!node)
    {
        return static_cast<int>(reinterpret_cast<std::intptr_t>(
            RtQuatRotate(quaternionAddress, axis, angle, 2)));
    }

    void* reference = CSF_AT(void*, node, 0x24);
    if (!reference)
    {
        return static_cast<int>(reinterpret_cast<std::intptr_t>(
            RtQuatRotate(quaternionAddress, axis, angle, 2)));
    }

    CSF_AT(float, frame, 0x18) =
        CSF_AT(float, frame, 0x18) - CSF_AT(float, reference, 0x10);
    CSF_AT(float, frame, 0x1C) =
        CSF_AT(float, frame, 0x1C) - CSF_AT(float, reference, 0x14);
    CSF_AT(float, frame, 0x20) =
        CSF_AT(float, frame, 0x20) - CSF_AT(float, reference, 0x18);

    if (CSF_AT(float, reference, 0xC) < g_TransitionOne)
    {
        const float referenceAngle =
            static_cast<float>(GfxHAnim_GetQuaternionAngle(reference, nullptr));
        alignas(8) std::uint8_t axisBytes[0xC];
        float* referenceAxis = GfxHAnim_GetQuaternionAxis(
            reference, nullptr, reinterpret_cast<float*>(axisBytes));
        const float inverseAngle = referenceAngle * g_NegativeRadiansToDegrees;
        RtQuatRotate(quaternionAddress, referenceAxis, inverseAngle, 2);
    }

    float* result = RtQuatRotate(quaternionAddress, axis, angle, 2);

    CSF_AT(float, frame, 0x18) =
        CSF_AT(float, reference, 0x10) + CSF_AT(float, frame, 0x18);
    CSF_AT(float, frame, 0x1C) =
        CSF_AT(float, reference, 0x14) + CSF_AT(float, frame, 0x1C);
    CSF_AT(float, frame, 0x20) =
        CSF_AT(float, reference, 0x18) + CSF_AT(float, frame, 0x20);

    return static_cast<int>(reinterpret_cast<std::intptr_t>(result));
}

HOOK(0x006E9B00, C_00861e98_RotateNode, "auto");
