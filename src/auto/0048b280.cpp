#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453070, 0x00453070, (CGuiControl* self, void* , void* a2, float* a3, int* a4, int a5, float a6));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __stdcall, FindNearestAvailableBoxFaceId, 0x0048AEE0, (void* owner, float* position));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Get_Global_8B0578, 0x005189D0, ());
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// The packet also tags the array-growth call address as a vtable reference.
static auto const kArrayGrowthAddressTag = CSF_VTBL(0x0041E5F0);

static int __fastcall S_0048b280_SubmitSceneEffect(
    S_0048b280* self, void* /*edx*/, float* effect)
{
    if (!CSF_AT(uint32_t, effect, 0x20))
        return 0;

    float* position = effect + 1;

    alignas(8) uint8_t queryBytes[0x10];
    float* query = reinterpret_cast<float*>(queryBytes);
    query[0] = position[0];
    query[1] = position[1];
    query[2] = position[2];
    query[3] = effect[6];

    alignas(8) uint8_t resultsBytes[0x10];
    CSF_AT(uint32_t, resultsBytes, 0x00) = 0;
    CSF_AT(uint32_t, resultsBytes, 0x04) = 0;
    CSF_AT(uint32_t, resultsBytes, 0x08) = 0;
    CSF_AT(int, resultsBytes, 0x0C) = 10;

    alignas(8) uint8_t filterBytes[0x34];
    CSF_AT(int, filterBytes, 0x00) = 2865;
    std::memset(filterBytes + 0x04, 255, 12);
    CSF_AT(uint32_t, filterBytes, 0x10) = 0;
    CSF_AT(uint32_t, filterBytes, 0x14) = 0;
    CSF_AT(int, filterBytes, 0x18) = 0;
    CSF_AT(int, filterBytes, 0x1C) = 10;
    CSF_AT(uint32_t, filterBytes, 0x20) = 0;
    CSF_AT(uint32_t, filterBytes, 0x24) = 0;
    CSF_AT(uint32_t, filterBytes, 0x28) = 0;
    CSF_AT(int, filterBytes, 0x2C) = -1;
    CSF_AT(uint32_t, filterBytes, 0x30) = 0;

    S_0041e5f0_GrowArrayCapacity(
        reinterpret_cast<S_0041e5f0*>(filterBytes + 0x10), nullptr, 10);

    int insertionIndex = CSF_AT(int, filterBytes, 0x18);
    uint32_t* excludedObjects =
        reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, filterBytes, 0x10));
    uint32_t sourceObject = CSF_AT(uint32_t, effect, 0x00);
    CSF_AT(int, filterBytes, 0x18) = insertionIndex + 1;
    excludedObjects[insertionIndex] = sourceObject;

    alignas(8) uint8_t queryFlagsBytes[0x04];
    CSF_AT(int, queryFlagsBytes, 0) = 1;
    CGuiControl_ProcessDisplayFlags_453070(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        resultsBytes, query, reinterpret_cast<int*>(filterBytes),
        reinterpret_cast<int>(queryFlagsBytes), 0.0f);

    uint32_t resultCount = CSF_AT(uint32_t, resultsBytes, 0x08);
    if (resultCount)
    {
        alignas(8) uint8_t notificationBytes[0x3C];
        CSF_AT(uint32_t, notificationBytes, 0x10) =
            CSF_AT(uint32_t, effect, 0x20);
        CSF_AT(uint32_t, notificationBytes, 0x28) =
            CSF_AT(uint32_t, notificationBytes, 0x10);
        CSF_AT(uint32_t, notificationBytes, 0x0C) =
            CSF_AT(uint32_t, effect, 0x14);
        CSF_AT(uint32_t, notificationBytes, 0x18) =
            CSF_AT(uint32_t, position, 0x00);
        CSF_AT(uint32_t, notificationBytes, 0x1C) =
            CSF_AT(uint32_t, position, 0x04);
        CSF_AT(uint32_t, notificationBytes, 0x20) =
            CSF_AT(uint32_t, position, 0x08);
        CSF_AT(void**, notificationBytes, 0x00) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, notificationBytes, 0x04) = uint32_t(-1);
        CSF_AT(uint32_t, notificationBytes, 0x08) = uint32_t(-1);
        CSF_AT(int, notificationBytes, 0x14) = 0;
        CSF_AT(int, notificationBytes, 0x24) = 4;
        CSF_AT(S_0048b280*, notificationBytes, 0x2C) = self;

        void* source = reinterpret_cast<void*>(CSF_AT(uint32_t, effect, 0));
        if (source && (CSF_AT(uint8_t, source, 0x170) & 1))
        {
            bool removable = static_cast<uint8_t>(
                GameConfig_IsSceneObjectRemovable(
                    CSF_AT(int, source, 0x50))) != 0;
            uint8_t* ownerData = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, effect, 0)) + (removable ? 392 : 28);
            CSF_AT(uint32_t, notificationBytes, 0x04) =
                CSF_AT(uint32_t, ownerData, 0x04);
            CSF_AT(uint32_t, notificationBytes, 0x08) =
                CSF_AT(uint32_t, ownerData, 0x08);
        }

        // The apparent stack objects at -0x108 and -0xBC are one 0x74-byte record.
        alignas(8) uint8_t spawnBytes[0x74];
        C_0082e0f4& spawn = *reinterpret_cast<C_0082e0f4*>(spawnBytes);
        C_0082e0f4_InitializeSpawnRecord(&spawn, nullptr, 4);
        spawn.field_000c = CSF_AT(uint32_t, notificationBytes, 0x04);
        spawn.field_0034 = CSF_AT(uint32_t, position, 0x00);
        uint32_t effectId = CSF_AT(uint32_t, effect, 0x20);
        spawn.field_0010 = CSF_AT(uint32_t, notificationBytes, 0x08);
        spawn.field_006c = effectId;
        spawn.field_0060 = uint32_t(-1);
        spawn.field_0038 = CSF_AT(uint32_t, position, 0x04);
        spawn.field_003c = CSF_AT(uint32_t, position, 0x08);
        spawn.field_0071 = 1;

        for (uint32_t index = 0; index < resultCount; ++index)
        {
            C_008342e8* object =
                reinterpret_cast<C_008342e8**>(
                    CSF_AT(uint32_t, resultsBytes, 0))[index];

            CSF_AT(int, notificationBytes, 0x14) = 0;
            if (static_cast<uint8_t>(
                    GameConfig_IsResolvedEntryActive(object->field_0050)))
            {
                CSF_AT(int, notificationBytes, 0x14) =
                    FindNearestAvailableBoxFaceId(object, position);
            }

            void* listener = reinterpret_cast<void*>(object->field_0038);
            if (listener)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, listener, 0)[0x8C / 4])(
                        listener, nullptr, notificationBytes);
            }

            spawn.field_0018 = object->field_0020;
            spawn.field_001c = object->field_0024;
            float* objectPosition = GetObjectPositionVector(object, nullptr);
            float directionX = objectPosition[0] - position[0];
            float directionY = objectPosition[1] - position[1];
            float directionZ = objectPosition[2] - position[2];
            CSF_AT(float, notificationBytes, 0x30) = directionX;
            CSF_AT(float, notificationBytes, 0x34) = directionY;
            CSF_AT(float, notificationBytes, 0x38) = directionZ;
            std::memcpy(&spawn.field_004c, &directionX, sizeof(float));
            std::memcpy(&spawn.field_0050, &directionY, sizeof(float));
            std::memcpy(&spawn.field_0054, &directionZ, sizeof(float));

            if (directionX != g_ZeroVectorX ||
                directionY != g_ZeroVectorY ||
                directionZ != g_ZeroVectorZ)
            {
                NormalizeVector3(
                    reinterpret_cast<C_00833b88*>(&spawn.field_004c), nullptr);
            }

            int dispatcher = GetGlobalObjectField184();
            DispatchRecordToGlobalObject(
                reinterpret_cast<void*>(dispatcher), nullptr, &spawn, 1);
        }
    }

    Get_Global_8B0578();
    if (CSF_AT(uint32_t, filterBytes, 0x10))
    {
        CoreMemory_Free(
            reinterpret_cast<void*>(CSF_AT(uint32_t, filterBytes, 0x10)));
        CSF_AT(uint32_t, filterBytes, 0x10) = 0;
    }

    uint32_t resultBuffer = CSF_AT(uint32_t, resultsBytes, 0);
    if (resultBuffer)
        return CoreMemory_Free(reinterpret_cast<void*>(resultBuffer));
    return 0;
}

HOOK(0x0048B280, S_0048b280_SubmitSceneEffect, "auto");
