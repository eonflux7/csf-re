#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, SpatialQuery_InitializeQueryInfo, 0x0040E070, (void* self, void*));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, C_00834198_SetField0198, 0x0049CF30, (C_00834198* self, void*));
CSF_FUNC(void, __fastcall, WeaponFirearms_SetObjectDirection, 0x0049CF40, (void* self, void* edx, int* direction));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindPlayerById, 0x0073E2C0, (uint32_t playerId));
CSF_FUNC(char, __fastcall, CMenuMpStats_StoreStatsAndSetFlag, 0x00740320, (CMenuMpStats* self, void* , int firstValue, int secondValue, int thirdValue));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);

static float FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static int __fastcall C_00834198_SynchronizeLinkedState(
    C_00834198* self, void* /*edx*/, int /*unused*/)
{
    void* linkedObject = CSF_AT(void*, self, 0x18);
    auto* state = linkedObject
        ? reinterpret_cast<C_00833908*>(
              static_cast<uint8_t*>(linkedObject) - 120)
        : nullptr;

    alignas(8) uint8_t queryBytes[0x64];
    alignas(8) uint8_t positionBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];

    const bool linkedFlag =
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x8 / 4])(
                linkedObject, nullptr);

    if (!linkedFlag && !self->field_0198)
    {
        C_00834198_SetField0198(self, nullptr);

        CSF_AT(uint32_t, positionBytes, 0) = state->field_0048;
        CSF_AT(uint32_t, positionBytes, 4) = state->field_004c;
        CSF_AT(uint32_t, positionBytes, 8) = state->field_0050;
        CSF_AT(uint32_t, directionBytes, 0) = state->field_0054;
        std::memcpy(directionBytes + 4, &state->field_0058, 4);
        CSF_AT(uint32_t, directionBytes, 8) =
            CSF_AT(uint32_t, state, 0x5C);

        SceneLoader_ConfigureFocusEntry(
            self, nullptr, reinterpret_cast<int*>(positionBytes));

        const float x = CSF_AT(float, directionBytes, 0);
        const float y = CSF_AT(float, directionBytes, 4);
        const float z = CSF_AT(float, directionBytes, 8);
        if (z * z + y * y + x * x > g_fIntroVideoDuration)
        {
            WeaponFirearms_SetObjectDirection(
                self, nullptr, reinterpret_cast<int*>(directionBytes));
        }
        else
        {
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_004c), 0x77) = 0;
        }
    }

    if (state->field_0114)
    {
        SpatialQuery_InitializeQueryInfo(queryBytes, nullptr);

        const int queryType =
            *reinterpret_cast<const int8_t*>(&state->field_00d4);
        const uint32_t playerId = state->field_0110;
        const uint32_t queryFlags = state->field_00d8;
        CSF_AT(int, queryBytes, 0) = queryType;
        CSF_AT(uint32_t, queryBytes, 4) = queryFlags;

        GetGlobalDword_73C020();
        const int player = NetSession_FindPlayerById(playerId);
        int owner = 0;
        if (player)
        {
            const int value =
                GetValueIfNotFlagged(reinterpret_cast<void*>(player), nullptr);
            if (value)
                owner = value - 20;
        }
        CSF_AT(int, queryBytes, 0x4C) = owner;

        CSF_AT(uint32_t, queryBytes, 0x08) = state->field_00dc;
        const float scale = FloatFromBits(state->field_0100);
        const uint32_t value3 = state->field_00e0;
        const uint32_t value4 = state->field_00e4;

        CSF_AT(uint32_t, directionBytes, 0) = state->field_00f4;
        CSF_AT(uint32_t, queryBytes, 0x0C) = value3;
        CSF_AT(float, positionBytes, 0) =
            CSF_AT(float, directionBytes, 0) * scale;
        CSF_AT(uint32_t, directionBytes, 4) = state->field_00f8;
        CSF_AT(uint32_t, queryBytes, 0x10) = value4;
        CSF_AT(float, positionBytes, 4) =
            CSF_AT(float, directionBytes, 4) * scale;
        CSF_AT(uint32_t, directionBytes, 8) = state->field_00fc;

        CSF_AT(uint32_t, queryBytes, 0x20) =
            CSF_AT(uint32_t, positionBytes, 0);
        CSF_AT(uint32_t, queryBytes, 0x24) =
            CSF_AT(uint32_t, positionBytes, 4);
        CSF_AT(uint32_t, queryBytes, 0x2C) =
            CSF_AT(uint32_t, directionBytes, 0);

        const uint32_t value5 = state->field_00e8;
        CSF_AT(float, positionBytes, 8) =
            CSF_AT(float, directionBytes, 8) * scale;
        CSF_AT(uint32_t, queryBytes, 0x28) =
            CSF_AT(uint32_t, positionBytes, 8);
        CSF_AT(uint32_t, queryBytes, 0x30) =
            CSF_AT(uint32_t, directionBytes, 4);

        const uint32_t value6 = state->field_00ec;
        const uint32_t auxiliaryX = state->field_0104;
        CSF_AT(uint32_t, queryBytes, 0x34) =
            CSF_AT(uint32_t, directionBytes, 8);
        const uint32_t value7 = state->field_00f0;
        CSF_AT(uint32_t, queryBytes, 0x18) = value6;
        const uint32_t auxiliaryY = state->field_0108;
        CSF_AT(uint32_t, queryBytes, 0x14) = value5;

        void* entry = reinterpret_cast<void*>(self->field_004c);
        CSF_AT(uint32_t, queryBytes, 0x1C) = value7;
        const uint32_t auxiliaryZ = state->field_010c;
        CSF_AT(uint8_t, queryBytes, 0x48) = 1;
        CSF_AT(uint32_t, entry, 0x84) = auxiliaryX;
        CSF_AT(uint32_t, entry, 0x88) = auxiliaryY;
        CSF_AT(uint32_t, entry, 0x8C) = auxiliaryZ;

        entry = reinterpret_cast<void*>(self->field_004c);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, entry, 0)[0x50 / 4])(
                entry, nullptr, queryBytes);
    }

    state->field_0114 = 0;
    void* entry = reinterpret_cast<void*>(self->field_004c);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, entry, 0)[0x4C / 4])(entry, nullptr);

    if (static_cast<uint8_t>(result) == 0 && !self->field_0224)
    {
        self->field_0224 = 1;
        int* firstPosition = GetObjectPositionVector(self, nullptr);
        int* secondPosition = GetObjectPositionVector(self, nullptr);
        const int z = CSF_AT(int, secondPosition, 8);
        int* thirdPosition = GetObjectPositionVector(self, nullptr);
        const int y = CSF_AT(int, thirdPosition, 4);
        const int x = CSF_AT(int, firstPosition, 0);
        const char stored = CMenuMpStats_StoreStatsAndSetFlag(
            reinterpret_cast<CMenuMpStats*>(state), nullptr, x, y, z);
        result = static_cast<int>(
            (static_cast<uint32_t>(result) & 0xFFFFFF00u) |
            static_cast<uint8_t>(stored));
    }
    return result;
}

HOOK(0x0049E840, C_00834198_SynchronizeLinkedState, "auto");
