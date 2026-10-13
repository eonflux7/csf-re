#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(char, __fastcall, C_0082f9f8_BeginTaskAtTarget, 0x00420A70, (C_0082f9f8* self, void* , uint32_t* target, int param));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(bool, __fastcall, IsEntryFrameBlocked, 0x0049ED50, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiMountTasks_PrepareEntryFrame, 0x0049EDC0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, AiMountTasks_CollectEntryFrames, 0x0049F2C0, (void* self, void* edx, int a2, void* a3));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_GetSceneEntryCount, 0x0052E420, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_MountDegreesToRadians, 0x00834818);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_00834fa8_BeginAtNearestAvailableEntry(
    C_00834fa8* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x14) = 0;
    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, CSF_AT(int, self, 0x0C) + 256));
    if (!mount)
        return 0;

    auto* entries = reinterpret_cast<CMenuMpGameMode*>(mount->field_004c);
    const int entryCount = AiCore_GetSceneEntryCount(entries, nullptr);
    float nearestDistance = -1.0f;
    int selectedEntry = -1;
    int selectedFrame = -1;
    uint32_t frameOffset = 0;
    float targetAngle = static_cast<float>(
        C_008342e8_ComputeResolvedObjectScalar(mount, nullptr));

    alignas(8) uint8_t targetBytes[0x44];
    CSF_AT(float, targetBytes, 0x00) = 0.0f;
    CSF_AT(float, targetBytes, 0x04) = 0.0f;
    CSF_AT(float, targetBytes, 0x08) = 0.0f;

    for (int entryIndex = 0; entryIndex < entryCount; ++entryIndex)
    {
        void* entry = reinterpret_cast<void*>(
            CMenuMpGameMode_GetSceneEntry(entries, nullptr, entryIndex));
        void* registry = reinterpret_cast<void*>(mount->vGetField016c());
        const int objectIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, registry, 0)[0x98 / 4])(
                    registry, nullptr, CSF_AT(int, entry, 0x04));

        void* entryObject = nullptr;
        if (objectIndex != -1)
        {
            registry = reinterpret_cast<void*>(mount->vGetField016c());
            entryObject =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, registry, 0)[0x8C / 4])(
                        registry, nullptr, objectIndex);
        }

        if (objectIndex == -1 || !entryObject)
        {
            void* owner = CSF_AT(void*, self, 0x08);
            C_00833908_GetTargetName(
                CSF_AT(C_00833908*, owner, 0x118), nullptr);
            continue;
        }

        void* transform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, entryObject, 0)[0x18 / 4])(
                    entryObject, nullptr);
        float* entryPosition =
            reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x10 / 4])(
                    transform, nullptr);
        const float entryX = entryPosition[0];
        const float entryY = entryPosition[1];
        const float entryZ = entryPosition[2];

        void* owner = CSF_AT(void*, self, 0x08);
        float* actorPosition = GetObjectPositionVector(
            CSF_AT(void*, owner, 0x118), nullptr);
        const float dx = actorPosition[0] - entryX;
        const float dy = actorPosition[1] - entryY;
        const float dz = actorPosition[2] - entryZ;
        const float distance = dz * dz + dy * dy + dx * dx;

        if (nearestDistance == g_MissingMetricValue ||
            distance < nearestDistance)
        {
            alignas(8) uint8_t frameListBytes[0x10];
            CSF_AT(uint32_t, frameListBytes, 0x00) = 0;
            CSF_AT(uint32_t, frameListBytes, 0x04) = 0;
            CSF_AT(uint32_t, frameListBytes, 0x08) = 0;
            CSF_AT(int, frameListBytes, 0x0C) = 10;
            int availableFrame = -1;
            AiMountTasks_CollectEntryFrames(
                mount, nullptr, entryIndex, frameListBytes);

            uint32_t frameIndex = 0;
            if (CSF_AT(uint32_t, frameListBytes, 0x08))
            {
                while (availableFrame == -1)
                {
                    int* frames = CSF_AT(int*, frameListBytes, 0x00);
                    if (!IsEntryFrameBlocked(
                            mount, nullptr, frames[frameIndex]))
                    {
                        availableFrame =
                            static_cast<int>(frameIndex + frameOffset);
                    }

                    ++frameIndex;
                    if (frameIndex >=
                        CSF_AT(uint32_t, frameListBytes, 0x08))
                        break;
                }

                if (availableFrame != -1)
                {
                    selectedEntry = entryIndex;
                    nearestDistance = distance;
                    CSF_AT(float, targetBytes, 0x04) = entryY;
                    CSF_AT(float, targetBytes, 0x00) = entryX;
                    CSF_AT(float, targetBytes, 0x08) = entryZ;
                    selectedFrame = availableFrame;

                    transform =
                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, entryObject, 0)[0x18 / 4])(
                                entryObject, nullptr);
                    targetAngle = static_cast<float>(
                        reinterpret_cast<double(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, transform, 0)[0x28 / 4])(
                                transform, nullptr));
                }
            }

            frameOffset += CSF_AT(uint32_t, frameListBytes, 0x08);
            if (CSF_AT(void*, frameListBytes, 0x00))
            {
                Mem_Free(CSF_AT(void*, frameListBytes, 0x00));
                CSF_AT(uint32_t, frameListBytes, 0x00) = 0;
            }
        }
    }

    if (selectedEntry == -1)
        CSF_AT(uint8_t, self, 0x14) = 1;

    if (CSF_AT(uint8_t, self, 0x14) || selectedFrame == -1)
    {
        return static_cast<char>(CharTask_LookupByOffset(
            self, nullptr, CSF_AT(int, self, 0x0C) + 236));
    }

    CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C) = selectedEntry;
    CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110) = selectedFrame;

    void* entry = reinterpret_cast<void*>(CMenuMpGameMode_GetSceneEntry(
        entries, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C)));
    if (!CSF_AT(uint8_t, entry, 0x0C) &&
        !CSF_AT(uint8_t, entry, 0x0D) &&
        (CSF_AT(int, entry, 0x10) != -1 ||
         CSF_AT(int, entry, 0x14) != -1))
    {
        entries->vHandleLinkedItemChange(
            CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C));
    }

    AiMountTasks_PrepareEntryFrame(
        mount, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110),
        CSF_AT(int, CSF_AT(void*, self, 0x08), 0x118) + 28);

    auto* movement = CSF_AT(
        C_0082f9f8*,
        CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), 0x30);
    const float angleRadians = static_cast<float>(
        (C_008342e8_ComputeResolvedObjectScalar(mount, nullptr) +
         targetAngle) * g_MountDegreesToRadians);
    int angleParameter;
    std::memcpy(&angleParameter, &angleRadians, sizeof(angleParameter));
    return C_0082f9f8_BeginTaskAtTarget(
        movement, nullptr, reinterpret_cast<uint32_t*>(targetBytes),
        angleParameter);
}

HOOK(0x004A7DC0, C_00834fa8_BeginAtNearestAvailableEntry, "auto");
