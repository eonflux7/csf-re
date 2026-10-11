#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAttackTasks_IsAttackDelayActive, 0x0047AEB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiAttackTasks_UpdateAttackDelay, 0x0047AED0, (void* self, void* edx, float a2));
CSF_FUNC(bool, __fastcall, AiAttackTasks_GetAttackMode, 0x0047BBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAttackTasks_StartTargetAttack, 0x0047CAB0, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, AiAttackTasks_UpdateWithoutFiring, 0x0047CBE0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiAttackTasks_CanAttackTarget, 0x0047D020, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAttackTasks_SetWeaponTarget, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, AiAttackTasks_IsWeaponActionAvailable, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int elapsedBits));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static void __fastcall TCharTask_UpdateAttack(TCharTask* self, void* /*edx*/, int elapsedBits)
{
    float elapsed;
    std::memcpy(&elapsed, &elapsedBits, sizeof(elapsed));

    TCharTask_BaseUpdate(self, nullptr, elapsedBits);

    if (static_cast<uint8_t>(CharActionFsm_IsWeaponActionBlocked(
            CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), nullptr)))
    {
        CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0x0C) + 60);
        return;
    }

    void* core = CSF_AT(void*, self, 0x08);
    if (CSF_AT(float, core, 0x2B4) > g_fZero)
        CSF_AT(float, core, 0x2B4) = CSF_AT(float, core, 0x2B4) - elapsed;

    AiAttackTasks_UpdateAttackDelay(
        static_cast<uint8_t*>(CSF_AT(void*, self, 0x08)) + 0x2D8,
        nullptr, elapsed);

    core = CSF_AT(void*, self, 0x08);
    const uint8_t hasOtherLinkedValue = static_cast<uint8_t>(
        HasNonzeroLinkedValueDifferentFrom(
            CSF_AT(void*, core, 0x14), nullptr, reinterpret_cast<int>(core)));

    core = CSF_AT(void*, self, 0x08);
    if (!hasOtherLinkedValue)
    {
        char* targetName = AiCore_GetTaskTargetNameBuffer(core, nullptr);
        AiAttackTasks_SetWeaponTarget(
            CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x204),
            nullptr, reinterpret_cast<int>(targetName + 16));

        void* weapon = CSF_AT(
            void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x44);

        if (((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, weapon, 0)[0x90 / 4])(weapon, nullptr))
        {
            if (!*AiCore_GetTaskTargetNameBuffer(CSF_AT(void*, self, 0x08), nullptr))
            {
                TFncGirar_OnRotationComplete(self, nullptr, 0);
                return;
            }

            void* items = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, weapon, 0)[0xD8 / 4])(weapon, nullptr);
            if (!GetCurrentItemCount(items, nullptr))
            {
                AiAttackTasks_UpdateWithoutFiring(CSF_AT(void*, self, 0x08), nullptr);
                CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0x0C) + 40);
                return;
            }
        }

        core = CSF_AT(void*, self, 0x08);
        if ((!CSF_AT(uint8_t, core, 0x2A8) ||
             !(CSF_AT(float, core, 0x2AC) >= CSF_AT(float, core, 0x2B0))) &&
            AiAttackTasks_CanAttackTarget(core, nullptr))
        {
            targetName = AiCore_GetTaskTargetNameBuffer(CSF_AT(void*, self, 0x08), nullptr);
            const int target = CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(targetName + 16));

            if (target)
            {
                if ((CSF_AT(uint32_t, reinterpret_cast<void*>(target), 0x50) ||
                     !(CSF_AT(float, CSF_AT(void*, self, 0x08), 0x2B4) > g_fZero)) &&
                    !static_cast<uint8_t>(AiAttackTasks_IsAttackDelayActive(
                        static_cast<uint8_t*>(CSF_AT(void*, self, 0x08)) + 0x2D8,
                        nullptr)) &&
                    AiAttackTasks_IsWeaponActionAvailable(
                        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x204),
                        nullptr, 7))
                {
                    const char attackMode = AiAttackTasks_GetAttackMode(
                        CSF_AT(void*, self, 0x08), nullptr);
                    targetName = AiCore_GetTaskTargetNameBuffer(
                        CSF_AT(void*, self, 0x08), nullptr);
                    AiAttackTasks_StartTargetAttack(
                        CSF_AT(void*, self, 0x08), nullptr,
                        reinterpret_cast<int>(targetName + 16), attackMode);
                    return;
                }
            }
        }

        core = CSF_AT(void*, self, 0x08);
    }

    AiAttackTasks_UpdateWithoutFiring(core, nullptr);
}

HOOK(0x0047D460, TCharTask_UpdateAttack, "auto");
