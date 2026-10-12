#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, AiTaskStates_GrowRegionEntries, 0x00448600,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));

static void* __fastcall AiTaskStates_AdjustPositionForRegion(
    void* self, void* /*edx*/, int* position, int regionId)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x44);
    uint32_t entryOffset = 0;

    for (uint32_t index = 0; index < entryCount; ++index, entryOffset += 16)
    {
        const uint32_t handleAddress =
            CSF_AT(uint32_t, self, 0x3C) + entryOffset;
        void* object = reinterpret_cast<void*>(static_cast<uint32_t>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                 static_cast<int>(handleAddress))));
        if (object)
        {
            void* region = CSF_AT(void*, object, 0x34);
            if (region && CSF_AT(int, region, 0x54) == regionId)
            {
                if (index == UINT32_MAX)
                    break;

                auto* entry = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, self, 0x3C) + 16 * index);
                CSF_AT(float, entry, 0x0C) = 1.0f;
                return region;
            }
        }
    }

    void* result = g_pCurrentScene;
    const uint32_t sceneCount = CSF_AT(uint32_t, result, 0x90);
    if (!sceneCount)
        return result;

    void** sceneEntries = CSF_AT(void**, result, 0x88);
    uint32_t sceneIndex = 0;
    for (; sceneIndex < sceneCount; ++sceneIndex)
    {
        result = sceneEntries[sceneIndex];
        if (result)
        {
            result = CSF_AT(void*, result, 0x34);
            if (result && CSF_AT(int, result, 0x54) == regionId)
                break;
        }
    }

    if (sceneIndex == sceneCount)
        return result;

    result = sceneEntries[sceneIndex];
    if (!result)
        return result;

    void* region = CSF_AT(void*, result, 0x34);
    const uint32_t flags = CSF_AT(uint32_t, region, 0x44);
    if ((flags & 0xC2) != 0 && (flags & 0x80) == 0)
        return result;

    const uint32_t handlePart1 = CSF_AT(uint32_t, result, 0x20);
    const uint32_t handlePart2 = CSF_AT(uint32_t, result, 0x24);

    if (CSF_AT(uint32_t, self, 0x44) >=
        CSF_AT(uint32_t, self, 0x40))
    {
        AiTaskStates_GrowRegionEntries(
            static_cast<uint8_t*>(self) + 0x3C, nullptr,
            CSF_AT(int, self, 0x48));
    }

    const uint32_t newIndex = CSF_AT(uint32_t, self, 0x44);
    auto* newEntry = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, self, 0x3C) + 16 * newIndex);
    CSF_AT(uint32_t, self, 0x44) = newIndex + 1;
    CSF_AT(uint32_t, newEntry, 0x08) = handlePart2;
    CSF_AT(uint32_t, newEntry, 0x04) = handlePart1;
    CSF_AT(float, newEntry, 0x0C) = 1.0f;

    alignas(8) uint8_t requestBytes[0x3C] = {};
    CSF_AT(int, requestBytes, 0x04) = -1;
    CSF_AT(int, requestBytes, 0x08) = -1;
    const int positionX = position[0];
    const int positionY = position[1];
    const int positionZ = position[2];
    CSF_AT(int, requestBytes, 0x14) = positionY;
    CSF_AT(int, requestBytes, 0x10) = positionX;

    void** regionVtable = CSF_AT(void**, region, 0);
    CSF_AT(void**, requestBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x20) = 7;
    CSF_AT(int, requestBytes, 0x18) = positionZ;
    CSF_AT(uint8_t, requestBytes, 0x0C) = 1;
    CSF_AT(float, requestBytes, 0x1C) = 270.0f;

    return ((void* (__fastcall*)(void*, void*, void*))regionVtable[0x44 / 4])(
        region, nullptr, requestBytes);
}

HOOK(0x00448DD0, AiTaskStates_AdjustPositionForRegion, "auto");
