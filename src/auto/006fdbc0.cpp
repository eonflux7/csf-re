#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SetValue18, 0x0050AAB0, (void* self, void* , int value));
CSF_FUNC(int, __cdecl, RwV3dTransformPoint, 0x00623010, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(double, __fastcall, GfxObjectProps_GetAttachmentHeight, 0x006B8360, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxFrames_ComposeMatrixChain, 0x006C0350, (void* a1, int a2));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int clumpHandle, int collectorHandle));

static int __fastcall GfxClumpLoader_ComputeAverageHeight(
    void* self, void* /*edx*/, void* destination)
{
    alignas(8) uint8_t matrixBytes[0x70] = {};
    alignas(8) uint8_t collectorBytes[0x28] = {};
    alignas(8) uint8_t pointBytes[0x10] = {};
    uint8_t* descriptor = matrixBytes + 0x40;

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* controller =
        reinterpret_cast<void*>(SceneLoader_GetSceneController(global, nullptr));
    void* referenceOwner =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);
    void* referencedObject = GetReferencedObject(referenceOwner, nullptr);

    if ((CSF_AT(uint8_t, referencedObject, 0xB) & 1) != 0)
    {
        int frameObject =
            GfxFrames_GetProviderFrameObject(CSF_AT(int, self, 0x8));
        void* attachment = reinterpret_cast<void*>(
            GfxObjectProps_GetObjectAttachment(frameObject));
        float height = static_cast<float>(
            GfxObjectProps_GetAttachmentHeight(attachment, nullptr));

        CSF_AT(int, descriptor, 0x0) = 0;
        CSF_AT(float, descriptor, 0x4) = height;
        CSF_AT(int, descriptor, 0x8) = 0;
        CSF_AT(int, descriptor, 0xC) = 0;
        CSF_AT(int, descriptor, 0x10) = 1065353216;
        CSF_AT(int, descriptor, 0x14) = 0;
        CSF_AT(uint8_t, descriptor, 0x28) = 1;
        std::memcpy(static_cast<uint8_t*>(destination) + 0x60,
                    descriptor, 0x2C);
        return 1065353216;
    }

    int result = GfxFrames_GetProviderFrameObject(CSF_AT(int, self, 0x8));
    if (!result)
        return result;

    void* frameObject = reinterpret_cast<void*>(result);
    void* geometry = CSF_AT(void*, frameObject, 0x18);
    if (!geometry)
        return result;

    int frame = CSF_AT(int, frameObject, 0x4);
    int triangleCount = CSF_AT(int, geometry, 0x10);
    uint16_t* triangles = CSF_AT(uint16_t*, geometry, 0x2C);
    void* morphTarget = CSF_AT(void*, geometry, 0x5C);
    uint8_t* vertices = CSF_AT(uint8_t*, morphTarget, 0x14);

    float heightSum = 0.0f;
    GfxFrames_ComposeMatrixChain(matrixBytes, frame);
    if (triangleCount > 0)
    {
        uint16_t* triangle = triangles;
        int remaining = triangleCount;
        do
        {
            uint16_t* index = triangle;
            int corners = 3;
            do
            {
                RwV3dTransformPoint(
                    reinterpret_cast<int>(pointBytes),
                    reinterpret_cast<int>(vertices + 12 * *index),
                    reinterpret_cast<int>(matrixBytes));
                ++index;
                --corners;
                heightSum = CSF_AT(float, pointBytes, 0x4) + heightSum;
            } while (corners);
            triangle += 4;
            --remaining;
        } while (remaining);
    }

    int vertexCount =
        static_cast<int>(3u * static_cast<uint32_t>(triangleCount));
    float averageHeight = heightSum / static_cast<float>(vertexCount);

    CSF_AT(int, descriptor, 0x0) = 0;
    CSF_AT(int, descriptor, 0x8) = 0;
    CSF_AT(float, pointBytes, 0x4) = 1.0f;
    CSF_AT(int, pointBytes, 0x0) = 0;
    CSF_AT(int, pointBytes, 0x8) = 0;
    CSF_AT(int, descriptor, 0xC) = 0;
    CSF_AT(int, descriptor, 0x14) = 0;
    CSF_AT(uint8_t, descriptor, 0x28) = 1;

    CSF_AT(int, collectorBytes, 0x0) = 0;
    CSF_AT(int, collectorBytes, 0x4) = 0;
    CSF_AT(uint32_t, collectorBytes, 0x8) = 0;
    CSF_AT(int, collectorBytes, 0xC) = 10;

    CSF_AT(float, descriptor, 0x4) = averageHeight;
    CSF_AT(int, descriptor, 0x10) = 1065353216;
    std::memcpy(static_cast<uint8_t*>(destination) + 0x60,
                descriptor, 0x2C);

    SceneLoader_CollectSectorObjects(
        CSF_AT(int, self, 0x8), reinterpret_cast<int>(collectorBytes));

    uint32_t index = 0;
    if (CSF_AT(uint32_t, collectorBytes, 0x8))
    {
        int heightBits;
        std::memcpy(&heightBits, &averageHeight, sizeof(heightBits));
        do
        {
            int* objects = CSF_AT(int*, collectorBytes, 0x0);
            void* attachment = reinterpret_cast<void*>(
                GfxObjectProps_GetObjectAttachment(objects[index]));
            SetValue18(attachment, nullptr, heightBits);
            ++index;
        } while (index < CSF_AT(uint32_t, collectorBytes, 0x8));
    }

    void* objects = CSF_AT(void*, collectorBytes, 0x0);
    if (objects)
        return Mem_Free(objects);
    return 0;
}

HOOK(0x006FDBC0, GfxClumpLoader_ComputeAverageHeight, "auto");
