#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, WaypointGraph_ArePointKeysEqual, 0x004A9BF0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(const char*, __fastcall, C_0082fbe8_GetConstantName, 0x0050F6D0, (C_0082fbe8* , void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ClearMember0C, 0x0075A1C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AppendThreeDwordRecord_75A3A0, 0x0075A3A0, (C_0085e7b8* self, void* , const uint32_t* record));
CSF_FUNC(int, __fastcall, GetCachedEntryObject, 0x0075C260, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SearchFacingTarget, 0x0075CE00, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, Table_GetRecord, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, WaypointGraph_LookupWaypoint, 0x0075CF20, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, WaypointGraph_BuildCachedPath, 0x0075FA10, (void* self, void* edx, float* a2, uint32_t* a3, int a4));

static char __fastcall S_0075f7c0_FindPath(
    S_0075f7c0* self, void* /*edx*/,
    float* origin, float* destination,
    C_0085e7b8* path, uint32_t* destinationValue)
{
    alignas(8) uint8_t originKeyBytes[0x8] = {};
    alignas(8) uint8_t destinationKeyBytes[0xC] = {};
    auto* originKey = reinterpret_cast<uint32_t*>(originKeyBytes);
    auto* destinationKey = reinterpret_cast<uint32_t*>(destinationKeyBytes);

    CSF_AT(char, destinationKeyBytes, 0xB) = 0;
    ClearMember0C(path, nullptr);

    if (origin[0] == g_fZero &&
        origin[1] == g_fZero &&
        origin[2] == g_fZero)
        return 0;

    if (destination[0] == g_fZero &&
        destination[1] == g_fZero &&
        destination[2] == g_fZero)
        return 0;

    CSF_AT(int32_t, originKeyBytes, 0x0) = -1;
    CSF_AT(int32_t, originKeyBytes, 0x4) = -1;
    CSF_AT(int32_t, destinationKeyBytes, 0x0) = -1;
    CSF_AT(int32_t, destinationKeyBytes, 0x4) = -1;

    if (!SearchFacingTarget(
            reinterpret_cast<C_0083e0d8*>(self->field_0000),
            nullptr, origin, originKey))
        return 0;

    if (!SearchFacingTarget(
            reinterpret_cast<C_0083e0d8*>(self->field_0000),
            nullptr, destination, destinationKey))
        return 0;

    char found;
    if (static_cast<uint8_t>(
            WaypointGraph_ArePointKeysEqual(originKey, nullptr, destinationKey)))
    {
        found = 1;
    }
    else
    {
        auto* cachedPath = reinterpret_cast<uint8_t*>(
            GetCachedEntryObject(
                reinterpret_cast<C_0083e0d8*>(self->field_0000), nullptr));

        WaypointGraph_BuildCachedPath(
            self, nullptr, reinterpret_cast<float*>(originKeyBytes),
            destinationKey, reinterpret_cast<int>(cachedPath));

        if (CSF_AT(uint32_t, cachedPath, 0x8))
        {
            uint32_t index = 0;
            do
            {
                auto* key = reinterpret_cast<uint32_t*>(
                    CSF_AT(uint8_t*, cachedPath, 0x0) + 8 * index);
                auto* waypoint = reinterpret_cast<uint8_t*>(
                    WaypointGraph_LookupWaypoint(
                        reinterpret_cast<C_0083e0d8*>(self->field_0000),
                        nullptr, key));
                AppendThreeDwordRecord_75A3A0(
                    path, nullptr,
                    reinterpret_cast<const uint32_t*>(waypoint + 4));
                ++index;
            }
            while (index < CSF_AT(uint32_t, cachedPath, 0x8));

            CSF_AT(char, destinationKeyBytes, 0xB) = 1;
        }
        else
        {
            auto* originWaypoint = reinterpret_cast<C_0082fbe8*>(
                WaypointGraph_LookupWaypoint(
                    reinterpret_cast<C_0083e0d8*>(self->field_0000),
                    nullptr, originKey));
            auto* destinationWaypoint = reinterpret_cast<C_0082fbe8*>(
                WaypointGraph_LookupWaypoint(
                    reinterpret_cast<C_0083e0d8*>(self->field_0000),
                    nullptr, destinationKey));

            if (originWaypoint && destinationWaypoint)
            {
                Table_GetRecord(
                    reinterpret_cast<C_0083e0d8*>(self->field_0000),
                    nullptr, CSF_AT(int32_t, originKeyBytes, 0x0));
                C_0082fbe8_GetConstantName(originWaypoint, nullptr);
                Table_GetRecord(
                    reinterpret_cast<C_0083e0d8*>(self->field_0000),
                    nullptr, CSF_AT(int32_t, destinationKeyBytes, 0x0));
                C_0082fbe8_GetConstantName(destinationWaypoint, nullptr);
            }
        }

        found = CSF_AT(char, destinationKeyBytes, 0xB);
        reinterpret_cast<C_0083e0d8*>(self->field_0000)->field_0030 = 0;
    }

    if (GetCachedObjectProvider(path, nullptr))
    {
        auto* waypoint = reinterpret_cast<uint8_t*>(
            WaypointGraph_LookupWaypoint(
                reinterpret_cast<C_0083e0d8*>(self->field_0000),
                nullptr, destinationKey));
        *destinationValue = CSF_AT(uint32_t, waypoint, 0x14);
    }

    return found;
}

HOOK(0x0075FC40, S_0075f7c0_FindPath, "auto");
