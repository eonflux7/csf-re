#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, SpatialQuery_InitializeQueryInfo, 0x0040E070, (void* self, void*));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, C_00834198_SetField0198, 0x0049CF30, (C_00834198* self, void*));
CSF_FUNC(void, __fastcall, WeaponFirearms_SetObjectDirection, 0x0049CF40, (void* self, void* edx, float* direction));
CSF_FUNC(char, __fastcall, NotifyDepleted_49D400, 0x0049D400, (void* self, void* edx, void* a3, int a4));
CSF_FUNC(int, __fastcall, SpatialQuery_BuildTransform, 0x0049E130, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindPlayerById, 0x0073E2C0, (uint32_t playerId));
CSF_FUNC(char, __fastcall, CMenuMpStats_StoreStatsAndActivate, 0x0073FA00, (CMenuMpStats* self, void* , int firstValue, int secondValue, int thirdValue));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);

static uint8_t __fastcall C_00834198_ApplyReplicatedState(
    C_00834198* self, void* /*edx*/, int /*unused*/)
{
    // The query header and its transform occupy one contiguous stack record.
    alignas(8) uint8_t queryBytes[0x20 + 0x44]{};
    alignas(8) uint8_t positionBytes[0xC]{};
    alignas(8) uint8_t directionBytes[0xC]{};

    void* controller = CSF_AT(void*, self, 0x18);
    uint8_t* state = controller
        ? static_cast<uint8_t*>(controller) - 112
        : nullptr;

    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[8 / 4])(controller, nullptr))
    {
        if (!self->field_0198)
        {
            C_00834198_SetField0198(self, nullptr);

            CSF_AT(uint32_t, positionBytes, 0) = CSF_AT(uint32_t, state, 0x4C);
            CSF_AT(uint32_t, positionBytes, 4) = CSF_AT(uint32_t, state, 0x50);
            CSF_AT(uint32_t, positionBytes, 8) = CSF_AT(uint32_t, state, 0x54);
            CSF_AT(uint32_t, directionBytes, 0) = CSF_AT(uint32_t, state, 0x58);
            CSF_AT(uint32_t, directionBytes, 4) = CSF_AT(uint32_t, state, 0x5C);
            CSF_AT(uint32_t, directionBytes, 8) = CSF_AT(uint32_t, state, 0x60);

            SceneLoader_ConfigureFocusEntry(
                self, nullptr, reinterpret_cast<int*>(positionBytes));

            const float lengthSquared =
                CSF_AT(float, directionBytes, 8) * CSF_AT(float, directionBytes, 8) +
                CSF_AT(float, directionBytes, 4) * CSF_AT(float, directionBytes, 4) +
                CSF_AT(float, directionBytes, 0) * CSF_AT(float, directionBytes, 0);

            // An unordered comparison takes the same path as a small vector.
            if (!(lengthSquared > g_fIntroVideoDuration))
            {
                CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_004c), 0x77) = 0;
            }
            else
            {
                WeaponFirearms_SetObjectDirection(
                    self, nullptr, reinterpret_cast<float*>(directionBytes));
            }
        }

        if (CSF_AT(uint8_t, state, 0xF8))
        {
            SpatialQuery_InitializeQueryInfo(queryBytes, nullptr);

            const int queryType = CSF_AT(int8_t, state, 0xB8);
            const uint32_t playerId = CSF_AT(uint32_t, state, 0xF4);
            const uint32_t queryFlags = CSF_AT(uint32_t, state, 0xBC);
            CSF_AT(int, queryBytes, 0) = queryType;
            CSF_AT(uint32_t, queryBytes, 4) = queryFlags;

            GetGlobalDword_73C020();
            const int player = NetSession_FindPlayerById(playerId);
            int playerObject = 0;
            if (player)
            {
                const int value = GetValueIfNotFlagged(
                    reinterpret_cast<void*>(player), nullptr);
                if (value)
                    playerObject = value - 20;
            }
            CSF_AT(int, queryBytes, 0x4C) = playerObject;

            CSF_AT(uint32_t, queryBytes, 8) = CSF_AT(uint32_t, state, 0xC0);
            CSF_AT(uint32_t, queryBytes, 0xC) = CSF_AT(uint32_t, state, 0xC4);
            CSF_AT(uint32_t, queryBytes, 0x10) = CSF_AT(uint32_t, state, 0xC8);

            CSF_AT(uint32_t, positionBytes, 0) = CSF_AT(uint32_t, state, 0xD8);
            CSF_AT(uint32_t, positionBytes, 4) = CSF_AT(uint32_t, state, 0xDC);
            CSF_AT(uint32_t, positionBytes, 8) = CSF_AT(uint32_t, state, 0xE0);
            SpatialQuery_BuildTransform(
                queryBytes + 0x20, nullptr,
                reinterpret_cast<float*>(positionBytes),
                CSF_AT(float, state, 0xE4));

            CSF_AT(uint32_t, queryBytes, 0x14) = CSF_AT(uint32_t, state, 0xCC);
            CSF_AT(uint32_t, queryBytes, 0x18) = CSF_AT(uint32_t, state, 0xD0);
            CSF_AT(uint32_t, queryBytes, 0x1C) = CSF_AT(uint32_t, state, 0xD4);
            CSF_AT(uint32_t, directionBytes, 0) = CSF_AT(uint32_t, state, 0xE8);
            CSF_AT(uint32_t, directionBytes, 4) = CSF_AT(uint32_t, state, 0xEC);

            void* child = reinterpret_cast<void*>(self->field_004c);
            CSF_AT(uint32_t, directionBytes, 8) = CSF_AT(uint32_t, state, 0xF0);
            CSF_AT(uint32_t, child, 0x84) = CSF_AT(uint32_t, directionBytes, 0);
            CSF_AT(uint32_t, child, 0x88) = CSF_AT(uint32_t, directionBytes, 4);
            CSF_AT(uint32_t, child, 0x8C) = CSF_AT(uint32_t, directionBytes, 8);

            child = reinterpret_cast<void*>(self->field_004c);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, child, 0)[0x50 / 4])(child, nullptr, queryBytes);
        }

        CSF_AT(uint8_t, state, 0xF8) = 0;

        void* child = reinterpret_cast<void*>(self->field_004c);
        if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x4C / 4])(child, nullptr) &&
            !self->field_0224)
        {
            self->field_0224 = 1;
            int* firstPosition = GetObjectPositionVector(self, nullptr);
            const int thirdValue = GetObjectPositionVector(self, nullptr)[2];
            int* lastPosition = GetObjectPositionVector(self, nullptr);
            const int secondValue = lastPosition[1];
            const int firstValue = firstPosition[0];
            CMenuMpStats_StoreStatsAndActivate(
                reinterpret_cast<CMenuMpStats*>(state), nullptr,
                firstValue, secondValue, thirdValue);
        }
    }

    uint8_t result = self->field_0184;
    if (!result)
    {
        result = CSF_AT(uint8_t, state, 0x98);
        CSF_AT(uint8_t, state, 0x98) = 0;
        if (result)
        {
            CSF_AT(uint32_t, positionBytes, 0) = CSF_AT(uint32_t, state, 0xA0);
            CSF_AT(uint32_t, positionBytes, 4) = CSF_AT(uint32_t, state, 0xA4);
            CSF_AT(uint32_t, positionBytes, 8) = CSF_AT(uint32_t, state, 0xA8);
            SceneLoader_ConfigureFocusEntry(
                self, nullptr, reinterpret_cast<int*>(positionBytes));

            CSF_AT(uint32_t, positionBytes, 0) = CSF_AT(uint32_t, state, 0xAC);
            CSF_AT(uint32_t, positionBytes, 4) = CSF_AT(uint32_t, state, 0xB0);
            CSF_AT(uint32_t, positionBytes, 8) = CSF_AT(uint32_t, state, 0xB4);
            return static_cast<uint8_t>(
                NotifyDepleted_49D400(self, nullptr, positionBytes, 1));
        }
    }
    return result;
}

HOOK(0x0049E5E0, C_00834198_ApplyReplicatedState, "auto");
