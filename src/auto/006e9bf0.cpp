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
CSF_GLOBAL(float, g_RadiansToDegrees, 0x00861DF8);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_00861e98_ApplyHierarchyTransformOffsets(
    C_00861e98* self, void* /*edx*/, int hierarchyAddress)
{
    auto* hierarchy = reinterpret_cast<uint8_t*>(hierarchyAddress);
    auto* range = CSF_AT(uint8_t*, hierarchy, 0x10);

    int nodeIndex = CSF_AT(int, range, 0x4);
    const int endIndex = static_cast<int>(
        static_cast<uint32_t>(nodeIndex) +
        static_cast<uint32_t>(CSF_AT(int, hierarchy, 0x4)));

    int result = nodeIndex;
    int frameIndex = 0;
    alignas(8) uint8_t axisBytes[0x1C];

    while (nodeIndex < endIndex)
    {
        auto* nodes = CSF_AT(uint8_t**, self, 0x4C);
        auto* node = nodes[nodeIndex];
        result = reinterpret_cast<int>(node);

        if (node)
        {
            auto* offset = CSF_AT(uint8_t*, node, 0x24);
            if (offset)
            {
                auto* frames = CSF_AT(uint8_t*, hierarchy, 0x20);
                result = reinterpret_cast<int>(frames);

                const uint32_t frameOffset =
                    static_cast<uint32_t>(CSF_AT(int, frames, 0x24)) *
                    static_cast<uint32_t>(frameIndex);
                auto* frame = frames + frameOffset + 0x4C;

                if (CSF_AT(float, offset, 0xC) < g_TransitionOne)
                {
                    const float angle = static_cast<float>(
                        GfxHAnim_GetQuaternionAngle(offset, nullptr));
                    float* axis = GfxHAnim_GetQuaternionAxis(
                        offset, nullptr, reinterpret_cast<float*>(axisBytes));

                    result = reinterpret_cast<int>(RtQuatRotate(
                        reinterpret_cast<int>(frame + 0x8),
                        axis, angle * g_RadiansToDegrees, 2));
                }

                if (CSF_AT(float, offset, 0x10) != g_ZeroVectorX ||
                    CSF_AT(float, offset, 0x14) != g_ZeroVectorY ||
                    CSF_AT(float, offset, 0x18) != g_ZeroVectorZ)
                {
                    CSF_AT(float, frame, 0x18) =
                        CSF_AT(float, offset, 0x10) +
                        CSF_AT(float, frame, 0x18);
                    CSF_AT(float, frame, 0x1C) =
                        CSF_AT(float, offset, 0x14) +
                        CSF_AT(float, frame, 0x1C);
                    CSF_AT(float, frame, 0x20) =
                        CSF_AT(float, offset, 0x18) +
                        CSF_AT(float, frame, 0x20);
                }
            }
        }

        nodeIndex = static_cast<int>(static_cast<uint32_t>(nodeIndex) + 1);
        frameIndex = static_cast<int>(static_cast<uint32_t>(frameIndex) + 1);
    }

    return result;
}

HOOK(0x006E9BF0, C_00861e98_ApplyHierarchyTransformOffsets, "auto");
