#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30, (int a1, float* a2, float a3, int a4));

static int __fastcall C_00861e98_RotateNode(
    C_00861e98* self, void* /*edx*/,
    void* animation, int nodeIndex, float* axis, float angle)
{
    auto* frames = CSF_AT(uint8_t*, animation, 0x20);
    void* hierarchy = CSF_AT(void*, animation, 0x10);
    const int firstNode = CSF_AT(int, hierarchy, 0x04);
    const int frameStride = CSF_AT(int, frames, 0x24);
    const int frameOffset = frameStride * (nodeIndex - firstNode);
    auto* frame = frames + frameOffset + 76;
    auto* quaternion = frames + frameOffset + 84;

    void* nodes = CSF_AT(void*, self, 0x4C);
    void* node = CSF_AT(void*, nodes, nodeIndex * 4);
    void* pivot = nullptr;
    if (node)
        pivot = CSF_AT(void*, node, 0x24);

    if (!pivot)
        return reinterpret_cast<int>(
            RtQuatRotate(reinterpret_cast<int>(quaternion), axis, angle, 2));

    CSF_AT(float, frame, 0x18) =
        CSF_AT(float, frame, 0x18) - CSF_AT(float, pivot, 0x10);
    CSF_AT(float, frame, 0x1C) =
        CSF_AT(float, frame, 0x1C) - CSF_AT(float, pivot, 0x14);
    CSF_AT(float, frame, 0x20) =
        CSF_AT(float, frame, 0x20) - CSF_AT(float, pivot, 0x18);

    const int result = reinterpret_cast<int>(
        RtQuatRotate(reinterpret_cast<int>(quaternion), axis, angle, 2));

    CSF_AT(float, frame, 0x18) =
        CSF_AT(float, pivot, 0x10) + CSF_AT(float, frame, 0x18);
    CSF_AT(float, frame, 0x1C) =
        CSF_AT(float, pivot, 0x14) + CSF_AT(float, frame, 0x1C);
    CSF_AT(float, frame, 0x20) =
        CSF_AT(float, pivot, 0x18) + CSF_AT(float, frame, 0x20);

    return result;
}

HOOK(0x006E8B20, C_00861e98_RotateNode, "auto");
