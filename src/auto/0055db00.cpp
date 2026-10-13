#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(std::uint8_t, __fastcall, CMenuPlayerInfo_UpdateFriendshipControls, 0x0055D4C0, (CMenuPlayerInfo* self, void*));
CSF_FUNC(void*, __fastcall, CMenuPlayerInfo_ProcessPendingAction, 0x0055D730, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuPlayerInfo_SetFriendRequestTextByState, 0x0055D880, (CMenuPlayerInfo* self, void*));
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void* __fastcall CMenuPlayerInfo_Update(
    CMenuPlayerInfo* self, void* /*edx*/, int deltaBits)
{
    void* control = reinterpret_cast<void*>(self->field_001c);
    if (control &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0xBC / 4])(control, nullptr))
    {
        control = reinterpret_cast<void*>(self->field_001c);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xB8 / 4])(control, nullptr, 1);
    }

    auto* timeState =
        reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(timeState, nullptr, deltaBits);
    SceneLoader_NoOp();

    switch (self->field_0038)
    {
    case 0:
    {
        float countdown;
        std::memcpy(&countdown, &self->field_0044, sizeof(countdown));

        // An unordered comparison follows the countdown branch too.
        if (countdown <= g_fZero)
        {
            const std::uint8_t result =
                CMenuPlayerInfo_UpdateFriendshipControls(self, nullptr);
            countdown = 3.0f;
            std::memcpy(&self->field_0044, &countdown, sizeof(countdown));
            return reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));
        }

        void* result = reinterpret_cast<void*>(self->field_0020);
        if (result)
        {
            float delta;
            std::memcpy(&delta, &deltaBits, sizeof(delta));
            std::memcpy(&countdown, &self->field_0044, sizeof(countdown));
            countdown -= delta;
            std::memcpy(&self->field_0044, &countdown, sizeof(countdown));
        }
        return result;
    }

    case 1:
    {
        void* global = reinterpret_cast<void*>(GetGlobal8B0688());
        void* service = CSF_AT(void*, global, 0x1484);
        const bool ready =
            reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, service, 0)[0x14 / 4])(service, nullptr);
        if (!ready)
            return nullptr;

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_ResetPending(state, nullptr);
        const int result =
            CMenuPlayerInfo_SetFriendRequestTextByState(self, nullptr);
        self->field_0038 = 0;
        return reinterpret_cast<void*>(result);
    }

    case 2:
    {
        auto* state =
            reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
        void* result = C_00861120_GetSchedulerProxy(state, nullptr);
        if (reinterpret_cast<std::uintptr_t>(result) == 1)
            self->field_0038 = 0;
        return result;
    }

    case 3:
        return CMenuPlayerInfo_ProcessPendingAction(self, nullptr);

    default:
        return reinterpret_cast<void*>(self->field_0038);
    }
}

HOOK(0x0055DB00, CMenuPlayerInfo_Update, "auto");
