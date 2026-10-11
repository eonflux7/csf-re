#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpHAnimFrameGetID, 0x0056CFA0, (int a1));
CSF_FUNC(char*, __cdecl, RtAnimAnimationCreate, 0x0061EE10, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorDestroy, 0x0061EE90, (int a1));
CSF_FUNC(char*, __cdecl, RtCompressedKeyConvertFromStd, 0x007C5D00, (int* a1));
CSF_FUNC(int, __cdecl, RtQuatConvertFromMatrix, 0x007D9D40, (float* a1, float* a2));
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0,
         (int frame, int (__cdecl* callback)(int, int), int data));
CSF_FUNC(int, __cdecl, HAnim_FindHierarchyCallback, 0x006E8010,
         (int frame, int data));
CSF_FUNC(int, __cdecl, HAnim_FindFrameByIdCallback, 0x006E8050,
         (int frame, int data));

static int __cdecl GfxHAnim_CreateBindPoseAnimation(int object)
{
    alignas(8) uint8_t hierarchyBytes[0x4];
    CSF_AT(void*, hierarchyBytes, 0) = nullptr;

    RwFrameForAllChildren(
        CSF_AT(int, reinterpret_cast<void*>(object), 0x4),
        HAnim_FindHierarchyCallback,
        reinterpret_cast<int>(hierarchyBytes));

    void* hierarchy = CSF_AT(void*, hierarchyBytes, 0);
    if (!hierarchy)
        return 0;

    char* animation = RtAnimAnimationCreate(
        1,
        static_cast<int>(2u * CSF_AT(uint32_t, hierarchy, 0x4)),
        0,
        1065353216);
    if (!animation)
        return 0;

    const int nodeCount = CSF_AT(int, hierarchy, 0x4);
    char* firstKey = CSF_AT(char*, animation, 0x10);
    char* secondKey = firstKey + 0x24 * nodeCount;

    if (nodeCount > 0)
    {
        alignas(8) uint8_t searchBytes[0x14];
        int nodeIndex = 0;
        uint32_t nodeOffset = 0;

        do
        {
            CSF_AT(uint32_t, firstKey, 0) = 0;
            CSF_AT(uint32_t, firstKey, 0x4) = 0;

            void* nodeInfo = CSF_AT(void*, hierarchy, 0x10);
            CSF_AT(int, searchBytes, 0) =
                CSF_AT(int, static_cast<uint8_t*>(nodeInfo) + nodeOffset, 0);
            CSF_AT(int, searchBytes, 0x4) = 0;

            const int rootFrame = CSF_AT(int, hierarchy, 0x14);
            const int rootId = RpHAnimFrameGetID(rootFrame);
            if (rootId == CSF_AT(int, searchBytes, 0))
                CSF_AT(int, searchBytes, 0x4) = rootFrame;
            else
                RwFrameForAllChildren(
                    rootFrame,
                    HAnim_FindFrameByIdCallback,
                    reinterpret_cast<int>(searchBytes));

            uint8_t* matrix =
                reinterpret_cast<uint8_t*>(CSF_AT(int, searchBytes, 0x4)) + 0x10;
            RtQuatConvertFromMatrix(
                reinterpret_cast<float*>(firstKey + 0x8),
                reinterpret_cast<float*>(matrix));

            CSF_AT(uint32_t, firstKey, 0x18) = CSF_AT(uint32_t, matrix, 0x30);
            CSF_AT(uint32_t, firstKey, 0x1C) = CSF_AT(uint32_t, matrix, 0x34);
            CSF_AT(uint32_t, firstKey, 0x20) = CSF_AT(uint32_t, matrix, 0x38);

            std::memcpy(secondKey, firstKey, 0x24);
            CSF_AT(char*, secondKey, 0) = firstKey;
            CSF_AT(uint32_t, secondKey, 0x4) = 1065353216;

            secondKey += 0x24;
            nodeOffset += 0x10;
            const int currentNodeCount =
                CSF_AT(int, CSF_AT(void*, hierarchyBytes, 0), 0x4);
            firstKey += 0x24;
            ++nodeIndex;
            if (nodeIndex >= currentNodeCount)
                break;

            hierarchy = CSF_AT(void*, hierarchyBytes, 0);
        } while (true);
    }

    char* compressed =
        RtCompressedKeyConvertFromStd(reinterpret_cast<int*>(animation));
    if (compressed)
    {
        RtAnimInterpolatorDestroy(reinterpret_cast<int>(animation));
        return reinterpret_cast<int>(compressed);
    }

    return reinterpret_cast<int>(animation);
}

HOOK(0x006E81A0, GfxHAnim_CreateBindPoseAnimation, "auto");
