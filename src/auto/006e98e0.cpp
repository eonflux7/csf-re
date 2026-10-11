#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, GfxMatrix_InitializeQuaternion, 0x006AE6D0, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, C_00861e98_GetNode, 0x006E8A50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_RotateLinkedNodes, 0x006E8B20, (void* self, void* edx, int a2, int a3, int a4, float a5));
CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30, (int a1, float* a2, float a3, int a4));

static int __fastcall C_00861e98_RotateNode(
    C_00861e98* self, void* /*edx*/, int nodeIndex, int axisAddress, float angle)
{
    void* sourceNode = CSF_AT(void**, self, 0x4C)[nodeIndex];
    void* quaternion = sourceNode ? CSF_AT(void*, sourceNode, 0x24) : nullptr;

    if (quaternion)
    {
        void* node = reinterpret_cast<void*>(
            C_00861e98_GetNode(self, nullptr, nodeIndex));

        alignas(8) uint8_t translationBytes[0x10];
        float* negativeTranslation = reinterpret_cast<float*>(translationBytes);
        float* translation = reinterpret_cast<float*>(
            static_cast<uint8_t*>(quaternion) + 0x10);

        negativeTranslation[0] = -translation[0];
        negativeTranslation[1] = -translation[1];
        negativeTranslation[2] = -translation[2];

        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, node, 0)[0x28 / 4])(
                node, nullptr, negativeTranslation);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, float)>(
            CSF_AT(void**, node, 0)[0x2C / 4])(
                node, nullptr, axisAddress, angle);
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, node, 0)[0x28 / 4])(
                node, nullptr, translation);
    }
    else
    {
        void* node = reinterpret_cast<void*>(
            C_00861e98_GetNode(self, nullptr, nodeIndex));
        reinterpret_cast<void(__fastcall*)(void*, void*, int, float)>(
            CSF_AT(void**, node, 0)[0x2C / 4])(
                node, nullptr, axisAddress, angle);
    }

    if (CSF_AT(uint32_t, self, 0x0C) > 0)
    {
        uint32_t index = 0;
        uint32_t entryOffset = 0;
        do
        {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x04) + entryOffset;
            void* link = CSF_AT(void*, entry, 0x04);
            if (link && nodeIndex >= CSF_AT(int, entry, 0x18))
            {
                C_00861e98_RotateLinkedNodes(
                    self, nullptr, CSF_AT(int, link, 0x08),
                    nodeIndex, axisAddress, angle);
                C_00861e98_RotateLinkedNodes(
                    self, nullptr,
                    CSF_AT(int, CSF_AT(void*, entry, 0x04), 0x0C),
                    nodeIndex, axisAddress, angle);
            }
            ++index;
            entryOffset += 36;
        } while (index < CSF_AT(uint32_t, self, 0x0C));
    }

    void* node = reinterpret_cast<void*>(
        C_00861e98_GetNode(self, nullptr, nodeIndex));
    if (!CSF_AT(void*, node, 0x24))
    {
        uint32_t* newQuaternion = static_cast<uint32_t*>(Mem_Alloc(28));
        if (newQuaternion)
        {
            GfxMatrix_InitializeQuaternion(newQuaternion, nullptr);
            CSF_AT(uint32_t, newQuaternion, 0x10) = 0;
            CSF_AT(uint32_t, newQuaternion, 0x14) = 0;
            CSF_AT(uint32_t, newQuaternion, 0x18) = 0;
        }
        CSF_AT(void*, node, 0x24) = newQuaternion;
    }

    return static_cast<int>(reinterpret_cast<intptr_t>(
        RtQuatRotate(CSF_AT(int, node, 0x24),
                     reinterpret_cast<float*>(axisAddress), angle, 2)));
}

HOOK(0x006E98E0, C_00861e98_RotateNode, "auto");
