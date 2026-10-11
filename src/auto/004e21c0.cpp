#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CGuiControl_AnyRightOtherSceneTrue, 0x00450660, (CGuiControl* self, void* , int excludedObject));
CSF_FUNC(int, __fastcall, IsDamageBlocked, 0x0048E210, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_ApplyRecoil, 0x00495500, (void* self, void* edx, float a1));
CSF_FUNC(int, __fastcall, SuppressesDamageRecoil, 0x00495510, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InitializeRecoilData, 0x004A8D20, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_0083e5e8_GetDamageThreshold, 0x004E1120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e160_FinishDamage, 0x004E1180, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_0083e5e8_UpdateDamageReaction, 0x004E1AC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083e5e8_ProcessDamageEffects, 0x004E1B50, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0083e5e8_ProcessDamageThreshold, 0x004E1FE0, (void* self, void* edx, int arg0));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D8B0, 0x0050D8B0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_0083e5e8_ApplyDamage(
    C_0083e5e8* self, void* /*edx*/, uint32_t* damage)
{
    self->field_0024 = damage[1];
    self->field_0028 = damage[2];

    int result = static_cast<int32_t>(self->field_0008);
    if (result <= 0)
        return result;

    result = IsDamageBlocked(
        reinterpret_cast<void*>(self->field_000c), nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    result = GetGlobalValue_8BC1E0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x90C) != 0)
        return result;

    result = GetGlobalValue_8BC1E0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x90C) != 0)
    {
        result = GetGlobalState_50D190();
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0) == 0)
            return result;
    }

    void* owner = reinterpret_cast<void*>(self->field_000c);
    if (damage[9] != 3 &&
        static_cast<uint8_t>(SuppressesDamageRecoil(owner, nullptr)) == 0)
    {
        alignas(8) uint8_t recoilBytes[0x1C] = {};

        CSF_AT(int, recoilBytes, 0x14) =
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x924);
        CSF_AT(int, recoilBytes, 0x18) =
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x928);
        CSF_AT(int, recoilBytes, 0x10) =
            CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x92C);

        InitializeRecoilData(
            recoilBytes, nullptr,
            CSF_AT(int, recoilBytes, 0x14),
            CSF_AT(int, recoilBytes, 0x18));
        CSF_AT(int, recoilBytes, 0x0C) = CSF_AT(int, recoilBytes, 0x10);
        CSF_AT(uint8_t, recoilBytes, 0x08) = 1;

        // The supplied float binding carries the stack object's address bits.
        uint32_t recoilAddress = reinterpret_cast<uint32_t>(recoilBytes);
        float recoilArgument;
        std::memcpy(&recoilArgument, &recoilAddress, sizeof(recoilArgument));
        WeaponFirearms_ApplyRecoil(owner, nullptr, recoilArgument);
    }

    int damageAmount = static_cast<int32_t>(damage[4]);
    if (static_cast<int32_t>(damage[2]) >= 0)
    {
        int source = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(damage));
        if (source != 0 &&
            CSF_AT(int, reinterpret_cast<void*>(source), 0x50) == 1)
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            float scale = MenuSystem_GetTransitionScale_50D8B0(state, nullptr);
            int64_t scaledDamage = static_cast<int64_t>(
                static_cast<double>(scale) *
                static_cast<double>(static_cast<int32_t>(damage[4])));
            damageAmount = static_cast<int32_t>(static_cast<uint32_t>(scaledDamage));
        }
    }

    self->field_0008 -= static_cast<uint32_t>(damageAmount);

    if (damage[9] == 4 && CSF_AT(uint8_t, owner, 0x228) == 0)
    {
        void* toggleState = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        GlobalValue4FBDE0_SetToggleMode(toggleState, nullptr, 2);
    }

    C_0083e5e8_ProcessDamageEffects(self, nullptr, reinterpret_cast<int>(damage));
    C_0083e5e8_UpdateDamageReaction(self, nullptr, reinterpret_cast<int>(damage));

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0 &&
        damage[5] == 1)
    {
        self->field_0008 = 0;
    }

    uint32_t ownerFlags = CSF_AT(uint32_t, owner, 0x2E0);
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0 ||
        (ownerFlags & 0x10000) != 0)
    {
        if (static_cast<int32_t>(self->field_0008) <= 0)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                self->vtbl[0xCC / 4])(self, nullptr, damage);
        }
    }
    else
    {
        char hasOtherSceneObject = CGuiControl_AnyRightOtherSceneTrue(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
            static_cast<int>(self->field_000c));
        int remainingHealth = static_cast<int32_t>(self->field_0008);
        if (hasOtherSceneObject == 0)
        {
            if (remainingHealth <= 0)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                    self->vtbl[0xCC / 4])(self, nullptr, damage);
                return C_0083e160_FinishDamage(self, nullptr, damage);
            }
        }
        else
        {
            if (remainingHealth <= 0)
                self->field_0008 = 1;

            bool suppressThresholdReaction =
                reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    self->vtbl[0xB4 / 4])(self, nullptr);
            if (!suppressThresholdReaction)
            {
                int threshold = C_0083e5e8_GetDamageThreshold(self, nullptr);
                if (static_cast<int32_t>(self->field_0008) < threshold)
                {
                    C_0083e5e8_ProcessDamageThreshold(
                        self, nullptr, reinterpret_cast<int>(damage));
                    return C_0083e160_FinishDamage(self, nullptr, damage);
                }
            }
        }
    }

    return C_0083e160_FinishDamage(self, nullptr, damage);
}

HOOK(0x004E21C0, C_0083e5e8_ApplyDamage, "auto");
