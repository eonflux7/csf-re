#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_004ad990_UpdateActiveCover, 0x004ADB70, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, S_004ad990_UpdateCoverProgress, 0x004ADBD0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, S_004ad990_ResetCombatOrderFields, 0x004B8C80, (S_004ad990* self, void*));
CSF_FUNC(bool, __fastcall, S_004ad990_TestAvailableQuery, 0x004BD800, (S_004ad990* self, void*));
CSF_FUNC(bool, __fastcall, CoverAttack_Sub_4C4890, 0x004C4890, (void* self, void* edx));

static void __fastcall S_004ad990_UpdateGoToCover(
    S_004ad990* self, void* /*edx*/, int deltaBits)
{
    float deltaTime;
    std::memcpy(&deltaTime, &deltaBits, sizeof(deltaTime));

    self->field_009c = deltaTime + self->field_009c;
    const float remainingTime = self->field_0094 - deltaTime;
    self->field_0094 = remainingTime;

    if (remainingTime <= g_fZero)
    {
        if (self->field_002c != static_cast<uint32_t>(-1))
        {
            const bool useAlternateHandle =
                static_cast<int32_t>(self->field_0054) < 0;
            const int32_t handleIndex = useAlternateHandle
                ? CSF_AT(int32_t, self, 0x3C)
                : static_cast<int32_t>(self->field_0054);

            if (handleIndex >= 0)
            {
                auto* handle = reinterpret_cast<uint8_t*>(self)
                    + (useAlternateHandle ? 0x34 : 0x4C);
                void* object = reinterpret_cast<void*>(
                    CHandleTable_Resolve(
                        g_pCurrentScene, nullptr,
                        reinterpret_cast<int>(handle)));

                if (!object
                    || static_cast<uint8_t>(
                        SceneObj_IsInactive(object, nullptr)) != 0
                    || static_cast<uint8_t>(
                        SceneObject_PassesFirstCheck(object, nullptr)) != 0)
                {
                    S_004ad990_ResetCombatOrderFields(self, nullptr);
                }
            }
        }

        CoverAttack_Sub_4C4890(self, nullptr);
        const bool available = S_004ad990_TestAvailableQuery(self, nullptr);
        if (self->field_00a8 || available)
        {
            self->field_009c = 0.0f;
            self->field_00a8 = available;
        }

        const float interval = deltaTime * g_ScoreboardRefreshInterval;
        // The unordered comparison also selects interval.
        self->field_0094 =
            interval < g_SoundActivityThreshold ? 0.2f : interval;
    }

    if (self->field_00a8)
        S_004ad990_UpdateActiveCover(self, nullptr, deltaBits);

    S_004ad990_UpdateCoverProgress(self, nullptr, deltaTime);
}

HOOK(0x004C4B80, S_004ad990_UpdateGoToCover, "auto");
