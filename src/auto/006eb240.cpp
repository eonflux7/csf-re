#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, GfxMatrix_InitializeQuaternion, 0x006AE6D0, (uint32_t* self, void*));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionAxisAngle, 0x006AE970, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(double, __fastcall, GfxHAnim_GetQuaternionAngle, 0x006AEA20, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxHAnim_GetQuaternionAxis, 0x006AEA30, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GfxHAnim_GetNode, 0x006E8A50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxHAnim_ApplyLinkedNodeRotation, 0x006E9B00, (void* self, void* edx, int a2, int a3, int a4, float a5));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_HAnimDegreesToRadians, 0x00861DFC);
CSF_GLOBAL(float, g_HAnimNegativeRadiansToDegrees, 0x00861F28);

static float* __fastcall C_00861e98_SetNodeAxisAngle(
    C_00861e98* self, void* /*edx*/, int nodeIndex, int axis, float angle)
{
    void* existingNode = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_004c), 4 * nodeIndex);
    float* previousRotation = nullptr;
    if (existingNode)
        previousRotation = CSF_AT(float*, existingNode, 0x24);

    if (previousRotation)
    {
        void* node = reinterpret_cast<void*>(
            GfxHAnim_GetNode(self, nullptr, nodeIndex));

        alignas(8) uint8_t vectorBytes[0x14];
        float* vector = reinterpret_cast<float*>(vectorBytes);
        float* translation = previousRotation + 4;
        vector[0] = -previousRotation[4];
        vector[1] = -previousRotation[5];
        vector[2] = -previousRotation[6];

        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, node, 0)[0x28 / 4])(node, nullptr, vector);

        if (previousRotation[3] < g_TransitionOne)
        {
            const double previousAngle =
                GfxHAnim_GetQuaternionAngle(previousRotation, nullptr);
            const float inverseAngle =
                previousAngle * g_HAnimNegativeRadiansToDegrees;
            void** rotationVtable = CSF_AT(void**, node, 0);
            float* previousAxis =
                GfxHAnim_GetQuaternionAxis(previousRotation, nullptr, vector);

            reinterpret_cast<void(__fastcall*)(void*, void*, void*, float)>(
                rotationVtable[0x2c / 4])(
                    node, nullptr, previousAxis, inverseAngle);
        }

        reinterpret_cast<void(__fastcall*)(void*, void*, void*, float)>(
            CSF_AT(void**, node, 0)[0x2c / 4])(
                node, nullptr, reinterpret_cast<void*>(axis), angle);

        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, node, 0)[0x28 / 4])(node, nullptr, translation);
    }
    else
    {
        void* node = reinterpret_cast<void*>(
            GfxHAnim_GetNode(self, nullptr, nodeIndex));
        reinterpret_cast<void(__fastcall*)(void*, void*, void*, float)>(
            CSF_AT(void**, node, 0)[0x2c / 4])(
                node, nullptr, reinterpret_cast<void*>(axis), angle);
    }

    if (self->field_000c)
    {
        uint32_t entryIndex = 0;
        uint32_t entryOffset = 0;
        do
        {
            uint8_t* entry =
                reinterpret_cast<uint8_t*>(self->field_0004) + entryOffset;
            void* link = CSF_AT(void*, entry, 0x04);
            if (link && nodeIndex >= CSF_AT(int, entry, 0x18))
            {
                GfxHAnim_ApplyLinkedNodeRotation(
                    self, nullptr, CSF_AT(int, link, 0x08),
                    nodeIndex, axis, angle);

                link = CSF_AT(void*, entry, 0x04);
                GfxHAnim_ApplyLinkedNodeRotation(
                    self, nullptr, CSF_AT(int, link, 0x0c),
                    nodeIndex, axis, angle);
            }
            ++entryIndex;
            entryOffset += 36;
        }
        while (entryIndex < self->field_000c);
    }

    void* node = reinterpret_cast<void*>(
        GfxHAnim_GetNode(self, nullptr, nodeIndex));
    if (!CSF_AT(void*, node, 0x24))
    {
        uint32_t* rotation = static_cast<uint32_t*>(Mem_Alloc(28));
        if (rotation)
        {
            GfxMatrix_InitializeQuaternion(rotation, nullptr);
            CSF_AT(uint32_t, rotation, 0x10) = 0;
            CSF_AT(uint32_t, rotation, 0x14) = 0;
            CSF_AT(uint32_t, rotation, 0x18) = 0;
        }
        CSF_AT(uint32_t*, node, 0x24) = rotation;
    }

    const float radians = angle * g_HAnimDegreesToRadians;
    return GfxMatrix_SetQuaternionAxisAngle(
        CSF_AT(float*, node, 0x24), nullptr,
        reinterpret_cast<S_00526d40*>(axis), radians);
}

HOOK(0x006EB240, C_00861e98_SetNodeAxisAngle, "auto");
