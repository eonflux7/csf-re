#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, InitializeSpawnRecord, 0x004164B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InitializeImpactRecord, 0x00474700, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, InitializeShotRecord, 0x00474750, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, WeaponFirearms_SetEffectPosition, 0x00476330, (void* self, void* , int effectId, const std::uint32_t* position, char enabled));
CSF_FUNC(int, __fastcall, WeaponFirearms_GetState, 0x00476360, (void* self, void* , std::uint32_t* value, std::uint32_t* vector, std::uint8_t* flag));
CSF_FUNC(bool, __stdcall, WeaponFirearms_ProcessTraceHit, 0x0048ABB0, (int arg0, uint32_t* a3, int a2));
CSF_FUNC(int, __fastcall, WeaponFirearms_ApplyAreaEffect, 0x0048B560, (void* self, void* edx, float a2, float* a3, float* a4));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_GetSurfaceId, 0x0048E320, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Get_Global_8B0578, 0x005189D0, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(double, __fastcall, CalculateSquaredVectorLength, 0x0068E300, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(double, __fastcall, GfxPlane_RayIntersectionT, 0x006AD8C0, (float* self, void* , float* rayOrigin, float* rayDirection));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));
CSF_FUNC(int, __cdecl, IsSubtitleOrSoundConfigId, 0x00755840, (int configId));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t FloatBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static float FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static void DispatchImpactRecord(void* record)
{
    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, record, 1);
}

static C_00833b88* __fastcall S_0048b280_TraceShotAndDispatchImpact(
    S_0048b280* self, void* /*edx*/, int rayAddress, uint32_t weaponId,
    int shotAddress, char emitShot, C_00833b88* ignoredObject)
{
    void* shot = reinterpret_cast<void*>(shotAddress);
    float* ray = reinterpret_cast<float*>(rayAddress);
    float* rayOrigin = ray + 3;

    alignas(8) uint8_t spawnBytes[0x80]{};
    C_0082e0f4& spawn = *reinterpret_cast<C_0082e0f4*>(spawnBytes);
    alignas(8) uint8_t recordBytes[0x50]{};
    C_00831638& record = *reinterpret_cast<C_00831638*>(recordBytes);
    alignas(8) uint8_t queryBytes[0x1C]{};
    S_00694cd0& query = *reinterpret_cast<S_00694cd0*>(queryBytes);
    alignas(8) uint8_t hitBytes[0x68]{};
    alignas(8) uint8_t differenceBytes[0x14]{};
    alignas(8) uint8_t selectedPositionBytes[0x23]{};
    float* selectedPosition = reinterpret_cast<float*>(selectedPositionBytes);
    alignas(8) uint8_t vectorBytes[0x10]{};
    float* vector = reinterpret_cast<float*>(vectorBytes);
    alignas(8) uint8_t stateValueBytes[0x4]{};
    alignas(8) uint8_t stateFlagBytes[0x1]{};

    // The trace API receives the adjacent start and end vectors together.
    alignas(8) uint8_t boundsBytes[0x18]{};
    float* bounds = reinterpret_cast<float*>(boundsBytes);
    float* endpoint = bounds + 3;
    alignas(8) uint8_t countBytes[0x8]{};
    uint32_t& sceneCount = CSF_AT(uint32_t, countBytes, 0);

    C_00833b88* bestObject = nullptr;
    float nearestDistanceSquared = 3.4028235e38f;
    uint32_t bestDetail = 0;
    int selectedSurfaceId = 0;

    void* surfaceTable = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD4);
    CSF_AT(void*, countBytes, 0x4) = surfaceTable;
    uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    C_00833b88* shooter = reinterpret_cast<C_00833b88*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, shotAddress));
    if (!shooter)
        return nullptr;

    bool menuShot = false;
    if (menuContext)
    {
        if (menuContext == reinterpret_cast<uintptr_t>(shooter)
            || CSF_AT(uint8_t, shot, 0x2F))
            menuShot = true;
    }

    void* weaponTable = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);
    void* weaponEntry =
        reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, weaponTable, 0)[8 / 4])(weaponTable, nullptr, weaponId);
    float* weaponData = reinterpret_cast<float*>(
        static_cast<uint8_t*>(weaponEntry) + 4);
    uint32_t weaponOwner = shooter->field_0044;
    bool specialShot = CSF_AT(int, shot, 0xC) == 2;
    void* firearmState = nullptr;

    if (weaponOwner && specialShot)
    {
        void* owner = reinterpret_cast<void*>(weaponOwner);
        int stateIndex = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, owner, 0)[192 / 4])(owner, nullptr, weaponId);
        firearmState = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, owner, 0)[200 / 4])(owner, nullptr, stateIndex);
        if (firearmState)
        {
            vector[0] = 0.0f;
            vector[1] = 0.0f;
            vector[2] = 0.0f;
            WeaponFirearms_GetState(
                firearmState, nullptr,
                reinterpret_cast<uint32_t*>(stateValueBytes),
                reinterpret_cast<uint32_t*>(vectorBytes), stateFlagBytes);
            float interval = stateFlagBytes[0] ? g_TransitionOne : 0.5f;
            if (CSF_AT(float, self, 0) - CSF_AT(float, stateValueBytes, 0) < interval)
            {
                if (stateFlagBytes[0])
                {
                    InitializeImpactRecord(&record.field_000c, nullptr, 5);
                    record.field_0018 = CSF_AT(uint32_t, shot, 0x4);
                    record.field_001c = CSF_AT(uint32_t, shot, 0x8);
                    record.field_0040 = CSF_AT(uint32_t, weaponData, 0x70);
                    std::memcpy(&record.field_0030, &weaponId, 4);
                    record.field_0044 = FloatBits(vector[0]);
                    record.field_0048 = FloatBits(vector[1]);
                    record.field_004c = FloatBits(vector[2]);
                    DispatchImpactRecord(&record.field_000c);
                }
                else
                {
                    WeaponFirearms_SetEffectPosition(
                        firearmState, nullptr, -1027080192,
                        reinterpret_cast<uint32_t*>(vectorBytes), 0);
                }
                return bestObject;
            }
        }
    }

    if (CSF_AT(uint8_t, self, 0x4))
    {
        if (specialShot)
            return bestObject;
    }
    else if (specialShot)
    {
        CSF_AT(uint8_t, self, 0x4) = 1;
    }

    int hitType = GetStateValue(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 100);
    sceneCount = CSF_AT(uint32_t, g_pCurrentScene, 0x18);
    float range = static_cast<float>(hitType) * g_ProgressScale
        * (CSF_AT(float, weaponData, 0xE8) - CSF_AT(float, weaponData, 0xE4))
        + CSF_AT(float, weaponData, 0xE4);
    float rangeSquared = range * range;

    for (uint32_t objectIndex = 0; objectIndex < sceneCount; ++objectIndex)
    {
        C_00833b88* candidate =
            CSF_AT(C_00833b88**, g_pCurrentScene, 0x10)[objectIndex];
        if (!candidate
            || !(candidate->field_0170 & 1)
            || candidate == ignoredObject
            || candidate == shooter
            || !candidate->vGetField016c()
            || !((candidate->field_0170 & 8)
                 || (menuShot && candidate->field_0050 == 17))
            || reinterpret_cast<int(__fastcall*)(void*, void*)>(
                candidate->vtbl[0x7C / 4])(candidate, nullptr))
            continue;

        if (menuShot)
        {
            if (!candidate->field_0050
                && !static_cast<uint8_t>(IsSubtitleOrSoundConfigId(candidate->field_0054)))
                continue;
        }
        else if ((candidate->field_0170 & 0x2000)
                 && (specialShot || candidate->field_0078 != CSF_AT(uint32_t, shot, 0x28)))
        {
            continue;
        }

        C_0082dd78* attachment =
            reinterpret_cast<C_0082dd78*>(candidate->field_004c);
        int objectSurfaceId = SceneObj_GetSurfaceId(candidate, nullptr);
        bool canQuery = attachment != nullptr;
        if (attachment)
        {
            if (objectSurfaceId != -1)
            {
                void* surface = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, surfaceTable, 0)[8 / 4])(
                        surfaceTable, nullptr, objectSurfaceId);
                canQuery = (CSF_AT(uint8_t, surface, 0x20) & 1) != 0;
            }
            if (canQuery)
            {
                void* geometry = reinterpret_cast<void*>(
                    C_0082dd78_DispatchReturnedObject(attachment, nullptr));
                std::memset(&query, 0, 12);
                query.field_000c = 10;
                reinterpret_cast<uint8_t*>(&query.field_0010)[0] = 0;

                bool intersects =
                    reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_00694cd0*, int)>(
                        CSF_AT(void**, geometry, 0)[64 / 4])(
                            geometry, nullptr, rayAddress, &query, 0);
                if (intersects && query.field_0008)
                {
                    int selectedHit = -1;
                    int lastSurfaceId = 0;
                    uint32_t hitIndex = 0;
                    uint32_t hitOffset = 0;
                    for (;;)
                    {
                        void* hits = reinterpret_cast<void*>(query.field_0000);
                        uint32_t surfaceKey = CSF_AT(uint32_t,
                            static_cast<uint8_t*>(hits) + hitOffset, 0x1C);
                        lastSurfaceId =
                            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                                CSF_AT(void**, surfaceTable, 0)[16 / 4])(
                                    surfaceTable, nullptr, surfaceKey);
                        bool usable = lastSurfaceId == -1;
                        if (!usable)
                        {
                            void* surface =
                                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                                    CSF_AT(void**, surfaceTable, 0)[8 / 4])(
                                        surfaceTable, nullptr, lastSurfaceId);
                            stateFlagBytes[0] = CSF_AT(uint8_t, surface, 0x20) & 1;
                            usable = stateFlagBytes[0] != 0;
                        }
                        if (usable)
                        {
                            if (!(candidate->field_0170 & 0x2000))
                            {
                                selectedHit = static_cast<int>(hitIndex);
                                break;
                            }
                            int kind = CSF_AT(int,
                                static_cast<uint8_t*>(reinterpret_cast<void*>(query.field_0000))
                                    + hitOffset, 0x18);
                            bool preferred = kind == 1 || kind == 4 || kind == 2;
                            if (preferred || selectedHit == -1)
                            {
                                selectedHit = static_cast<int>(hitIndex);
                                if (preferred)
                                    break;
                            }
                        }
                        hitOffset += 40;
                        if (++hitIndex >= query.field_0008)
                            break;
                    }

                    if (selectedHit != -1)
                    {
                        uint8_t* selectedHitData =
                            reinterpret_cast<uint8_t*>(query.field_0000) + 40 * selectedHit;
                        float* difference = BuildFacingVectorBetween(
                            reinterpret_cast<float*>(selectedHitData), nullptr,
                            reinterpret_cast<float*>(differenceBytes), rayOrigin);
                        vector[0] = difference[0];
                        vector[1] = difference[1];
                        vector[2] = difference[2];
                        double distanceSquared =
                            CalculateSquaredVectorLength(vectorBytes, nullptr);
                        if (distanceSquared <= rangeSquared
                            && distanceSquared < nearestDistanceSquared)
                        {
                            nearestDistanceSquared = static_cast<float>(distanceSquared);
                            selectedPosition[0] = CSF_AT(float, selectedHitData, 0);
                            selectedPosition[1] = CSF_AT(float, selectedHitData, 0x4);
                            selectedPosition[2] = CSF_AT(float, selectedHitData, 0x8);
                            hitType = CSF_AT(int, selectedHitData, 0x18);
                            selectedSurfaceId = lastSurfaceId == -1
                                ? objectSurfaceId : lastSurfaceId;
                            bestObject = candidate;
                            bestDetail = CSF_AT(uint32_t, selectedHitData, 0x20);
                        }
                    }
                }
                ResetEmbeddedMemberAt8(reinterpret_cast<void**>(&query), nullptr);
            }
        }
    }

    bounds[0] = rayOrigin[0];
    bounds[1] = rayOrigin[1];
    bounds[2] = rayOrigin[2];
    if (bestObject)
    {
        endpoint[0] = selectedPosition[0];
        endpoint[1] = selectedPosition[1];
        endpoint[2] = selectedPosition[2];
    }
    else
    {
        float distance = weaponData[58];
        endpoint[0] = distance * ray[0] + rayOrigin[0];
        endpoint[1] = distance * ray[1] + rayOrigin[1];
        endpoint[2] = distance * ray[2] + rayOrigin[2];
    }

    CSF_AT(float, hitBytes, 0) = 3.4028235e38f;
    CSF_AT(float, hitBytes, 0x4) = 0.0f;
    CSF_AT(float, hitBytes, 0x8) = 0.0f;
    CSF_AT(float, hitBytes, 0xC) = 0.0f;
    CSF_AT(int, hitBytes, 0x10) = 0;
    CSF_AT(int, hitBytes, 0x14) = 0;
    CSF_AT(int, hitBytes, 0x18) = 0;
    CSF_AT(int, hitBytes, 0x1C) = -1;
    CSF_AT(int8_t, hitBytes, 0x20) = -1;
    CSF_AT(int, hitBytes, 0x24) = 0;
    CSF_AT(uint8_t, hitBytes, 0x28) = 0;
    CSF_AT(int, hitBytes, 0x2C) = -1;
    CSF_AT(int8_t, hitBytes, 0x30) = -1;
    CSF_AT(int, hitBytes, 0x5C) = 0;
    CSF_AT(int, hitBytes, 0x60) = 0;
    CSF_AT(int, hitBytes, 0x64) = 10;

    InitializeSpawnRecord(&spawn, nullptr, 4);
    spawn.field_000c = CSF_AT(uint32_t, shot, 0x4);
    spawn.field_0010 = CSF_AT(uint32_t, shot, 0x8);
    spawn.field_0018 = -1;
    spawn.field_001c = -1;
    spawn.field_004c = FloatBits(ray[0]);
    spawn.field_0050 = FloatBits(ray[1]);
    spawn.field_0054 = FloatBits(ray[2]);
    spawn.field_0024 = weaponId;
    spawn.field_0060 = CSF_AT(uint32_t, weaponData, 0x70);
    spawn.field_006c = CSF_AT(uint32_t, weaponData, 0x114);
    spawn.field_0070 = 0;

    bool acceptedHit = false;
    bool crossesPlane = false;
    float planeDistance = -1.0f;
    void* planeSettings =
        CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);
    if (CSF_AT(uint8_t, planeSettings, 0x2C))
    {
        planeSettings =
            CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);
        vector[0] = 0.0f;
        vector[1] = CSF_AT(float, planeSettings, 0x30);
        vector[2] = 0.0f;
        record.field_0024 = 0;
        record.field_0028 = FloatBits(vector[1]);
        record.field_002c = 0;
        float axisX = g_AxisY;
        std::memcpy(&record.field_0030, &axisX, 4);
        record.field_0034 = FloatBits(g_AxisY_Y);
        record.field_0038 = FloatBits(g_AxisY_Z);
        reinterpret_cast<uint8_t*>(&record.field_004c)[0] = 1;
        planeDistance = static_cast<float>(GfxPlane_RayIntersectionT(
            reinterpret_cast<float*>(&record.field_0024), nullptr, rayOrigin, ray));
    }

    if (CSF_AT(uint8_t, shot, 0x2C))
    {
        void* controller = reinterpret_cast<void*>(SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        if (!(bounds[0] == endpoint[0] && bounds[1] == endpoint[1]
              && bounds[2] == endpoint[2]))
        {
            void* traceWorld = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[52 / 4])(controller, nullptr);
            if (traceWorld)
            {
                sceneCount = 1;
                void* iterator =
                    reinterpret_cast<void*(__fastcall*)(void*, void*, int, float*, uint32_t*, int)>(
                        CSF_AT(void**, traceWorld, 0)[52 / 4])(
                            traceWorld, nullptr, 0, bounds, &sceneCount, 73);
                if (iterator)
                {
                    for (;;)
                    {
                        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                            CSF_AT(void**, iterator, 0)[12 / 4])(
                                iterator, nullptr, reinterpret_cast<float*>(hitBytes));
                        acceptedHit = WeaponFirearms_ProcessTraceHit(
                            reinterpret_cast<int>(&spawn),
                            reinterpret_cast<uint32_t*>(hitBytes), shotAddress);
                        if (acceptedHit && planeDistance >= 0.0f
                            && CSF_AT(float, hitBytes, 0) > planeDistance)
                            crossesPlane = true;
                        reinterpret_cast<void(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, iterator, 0)[16 / 4])(iterator, nullptr);
                        if (acceptedHit)
                        {
                            endpoint[0] = CSF_AT(float, hitBytes, 0x4);
                            endpoint[1] = CSF_AT(float, hitBytes, 0x8);
                            endpoint[2] = CSF_AT(float, hitBytes, 0xC);
                            break;
                        }
                        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, iterator, 0)[8 / 4])(iterator, nullptr))
                            break;
                    }
                }
            }
        }
    }

    if ((!bestObject || acceptedHit) && weaponOwner && firearmState)
    {
        WeaponFirearms_SetEffectPosition(
            firearmState, nullptr, CSF_AT(int, self, 0),
            reinterpret_cast<uint32_t*>(endpoint), !acceptedHit && !bestObject);
    }

    if (crossesPlane)
    {
        void* table = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD4);
        void* globalSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        spawn.field_005c =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, table, 0)[16 / 4])(
                    table, nullptr, CSF_AT(uint32_t, globalSettings, 0x904));
        vector[0] = planeDistance * ray[0] + rayOrigin[0];
        vector[1] = planeDistance * ray[1] + rayOrigin[1];
        vector[2] = planeDistance * ray[2] + rayOrigin[2];
        spawn.field_0034 = FloatBits(vector[0]);
        spawn.field_0038 = FloatBits(vector[1]);
        spawn.field_003c = FloatBits(vector[2]);
        spawn.field_0040 = FloatBits(g_AxisY);
        spawn.field_0044 = FloatBits(g_AxisY_Y);
        spawn.field_0048 = FloatBits(g_AxisY_Z);
        DispatchImpactRecord(&spawn);
    }

    if (emitShot
        && CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        InitializeShotRecord(&record, nullptr, 16);
        record.field_000c = CSF_AT(uint32_t, shot, 0x4);
        record.field_0010 = CSF_AT(uint32_t, shot, 0x8);
        record.field_0034 = FloatBits(rayOrigin[0]);
        record.field_0038 = FloatBits(rayOrigin[1]);
        record.field_003c = FloatBits(rayOrigin[2]);
        record.field_0040 = FloatBits(ray[0]);
        record.field_0044 = FloatBits(ray[1]);
        record.field_0048 = FloatBits(ray[2]);
        record.field_0024 = weaponId;
        record.field_004c = weaponId;
        DispatchImpactRecord(&record);
    }

    if (weaponData[72] > 0.0f)
    {
        WeaponFirearms_ApplyAreaEffect(
            self, nullptr, FloatFromBits(reinterpret_cast<uint32_t>(shooter)),
            weaponData, endpoint);
    }

    void* finalRecord;
    if (acceptedHit || !bestObject)
    {
        record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        record.field_0020 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        record.field_0024 = -1;
        record.field_0028 = -1;
        record.field_002c = 0;
        std::memset(&record.field_0034, 0, 9);
        record.field_0018 = spawn.field_000c;
        record.field_001c = spawn.field_0010;
        std::memcpy(&record.field_0030, &spawn.field_0024, 4);
        record.field_0040 = spawn.field_0060;
        record.field_0044 = FloatBits(endpoint[0]);
        record.field_0010 = 5;
        record.field_000c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00831618));
        record.field_0048 = FloatBits(endpoint[1]);
        record.field_004c = FloatBits(endpoint[2]);
        finalRecord = &record.field_000c;
    }
    else
    {
        C_00833b88* target = bestObject;
        spawn.field_0058 = 0;
        spawn.field_0068 = bestDetail;
        int surfaceId = selectedSurfaceId;
        if (surfaceId == -1)
            surfaceId = SceneObj_GetSurfaceId(bestObject, nullptr);
        spawn.field_005c = surfaceId;
        spawn.field_0070 = 0;

        if ((target->field_0170 & 0x2000)
            || static_cast<uint8_t>(GameConfig_IsResolvedEntryActive(target->field_0050))
            || target->field_0050 == 28 || target->field_0050 == 17)
        {
            record.field_0028 = CSF_AT(uint32_t, shot, 0x4);
            record.field_002c = CSF_AT(uint32_t, shot, 0x8);
            uint32_t weaponProperty = CSF_AT(uint32_t, weaponData, 0x114);
            float weaponValue = weaponData[28];
            std::memcpy(&record.field_0030, &weaponValue, 4);
            record.field_0034 = weaponProperty;
            record.field_004c = weaponProperty;
            record.field_0038 = hitType;
            bool shooterIsTypeZero = shooter->field_0050 == 0;
            record.field_0024 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
            std::memset(&record.field_003c, 0, 16);
            if (!shooterIsTypeZero)
                record.field_0038 = 0;
            if (!SceneObj_IsInactive(target, nullptr))
            {
                void* recipient = reinterpret_cast<void*>(target->field_0038);
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                    CSF_AT(void**, recipient, 0)[140 / 4])(
                        recipient, nullptr, &record.field_0024);
            }
        }

        spawn.field_0018 = target->field_0020;
        vector[0] = -ray[0];
        spawn.field_001c = target->field_0024;
        vector[1] = -ray[1];
        vector[2] = -ray[2];
        spawn.field_0034 = FloatBits(selectedPosition[0]);
        spawn.field_0038 = FloatBits(selectedPosition[1]);
        spawn.field_003c = FloatBits(selectedPosition[2]);
        spawn.field_0040 = FloatBits(vector[0]);
        spawn.field_0044 = FloatBits(vector[1]);
        spawn.field_0048 = FloatBits(vector[2]);

        if (CSF_AT(uint32_t, shot, 0xC) == 1)
        {
            S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(&spawn.field_0040), nullptr,
                reinterpret_cast<uint32_t*>(selectedPosition));
            float verticalProduct = selectedPosition[1] * g_fZero;
            vector[0] = selectedPosition[2] - verticalProduct;
            vector[1] = selectedPosition[0] * g_fZero - selectedPosition[2] * g_fZero;
            vector[2] = verticalProduct - selectedPosition[0];
            NormalizeVector3(reinterpret_cast<C_00833b88*>(vectorBytes), nullptr);
            float sideways = static_cast<float>(GetStateValue(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 30) - 15);
            float upwards = static_cast<float>(GetStateValue(
                reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 30) - 15);
            float offsetX = vector[0] * sideways;
            float offsetY = vector[1] * sideways;
            float offsetZ = vector[2] * sideways;
            vector[0] = upwards * g_fZero;
            vector[1] = upwards;
            vector[2] = vector[0];
            offsetZ = vector[0] + offsetZ;
            spawn.field_0034 = FloatBits(
                offsetX + vector[0] + FloatFromBits(spawn.field_0034));
            spawn.field_0038 = FloatBits(
                FloatFromBits(spawn.field_0038) + vector[1] + offsetY);
            spawn.field_003c = FloatBits(FloatFromBits(spawn.field_003c) + offsetZ);
        }

        if ((target->field_0170 & 0x2000)
            || static_cast<uint8_t>(GameConfig_IsResolvedEntryActive(target->field_0050)))
        {
            spawn.field_0070 = 1;
        }
        else
        {
            bool specialTarget = target->field_0050 == 28;
            spawn.field_0070 = 0;
            if (specialTarget)
                spawn.field_0070 = 1;
        }
        spawn.field_0064 = static_cast<uint8_t>(hitType);
        finalRecord = &spawn;
    }

    DispatchImpactRecord(finalRecord);
    Get_Global_8B0578();
    return bestObject;
}

HOOK(0x0048B650, S_0048b280_TraceShotAndDispatchImpact, "auto");
