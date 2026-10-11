#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_47AEB0, 0x0047AEB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_47AED0, 0x0047AED0, (void* self, void* edx, float a2));
CSF_FUNC(bool, __fastcall, sub_47BBA0, 0x0047BBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_47CAB0, 0x0047CAB0, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, sub_47CBE0, 0x0047CBE0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_47D020, 0x0047D020, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_48D030, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, float deltaTime));
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Retain the packet's vtable annotation; execution uses the direct binding above.
[[maybe_unused]] static const auto kTaskTargetNameVtable = CSF_VTBL(0x0047AE10);

static uint32_t __fastcall TCharTask_UpdateAttack(
    TCharTask* self, void* /*edx*/, float deltaTime)
{
    TCharTask_BaseUpdate(self, nullptr, deltaTime);

    if (static_cast<uint8_t>(CharActionFsm_IsWeaponActionBlocked(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14),
            nullptr)))
    {
        return CharTask_LookupByOffset(self, nullptr, self->field_000c + 104);
    }

    const uint32_t result = self->field_0010;
    if (result)
        return result;

    void* attackState = reinterpret_cast<void*>(self->field_0008);
    if (CSF_AT(float, attackState, 0x2B4) > g_fZero)
        CSF_AT(float, attackState, 0x2B4) =
            CSF_AT(float, attackState, 0x2B4) - deltaTime;

    sub_47AED0(
        reinterpret_cast<uint8_t*>(self->field_0008) + 728,
        nullptr, deltaTime);

    attackState = reinterpret_cast<void*>(self->field_0008);
    if (static_cast<uint8_t>(HasNonzeroLinkedValueDifferentFrom(
            CSF_AT(void*, attackState, 0x14),
            nullptr, static_cast<int>(self->field_0008))))
    {
        return sub_47CBE0(reinterpret_cast<void*>(self->field_0008), nullptr);
    }

    if (!CSF_AT(uint8_t, self, 0x14) &&
        !sub_48D030(
            CSF_AT(void*,
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14),
                0x204),
            nullptr, 1))
    {
        return reinterpret_cast<uint32_t>(
            ResolveSubObject(self, nullptr, self->field_0008 + 96));
    }

    void* weapon = CSF_AT(void*,
        CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14),
        0x44);

    if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, weapon, 0)[0x90 / 4])(weapon, nullptr))
    {
        void* currentItem = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, weapon, 0)[0xD8 / 4])(weapon, nullptr);
        if (!GetCurrentItemCount(currentItem, nullptr))
        {
            sub_47CBE0(reinterpret_cast<void*>(self->field_0008), nullptr);
            return CharTask_LookupByOffset(
                self, nullptr, self->field_000c + 84);
        }
    }

    attackState = reinterpret_cast<void*>(self->field_0008);
    if (CSF_AT(uint8_t, attackState, 0x2A8) &&
        CSF_AT(float, attackState, 0x2AC) >=
            CSF_AT(float, attackState, 0x2B0))
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
    }
    else
    {
        if (!sub_47D020(attackState, nullptr))
        {
            CharTask_LookupByOffset(self, nullptr, self->field_000c + 20);
            return sub_47CBE0(
                reinterpret_cast<void*>(self->field_0008), nullptr);
        }

        char* targetName = AiCore_GetTaskTargetNameBuffer(
            reinterpret_cast<void*>(self->field_0008), nullptr);
        const int target = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(targetName + 16));

        if (target &&
            (CSF_AT(uint32_t, reinterpret_cast<void*>(target), 0x50) ||
             !(CSF_AT(float, reinterpret_cast<void*>(self->field_0008), 0x2B4)
                 > g_fZero)) &&
            !static_cast<uint8_t>(sub_47AEB0(
                reinterpret_cast<uint8_t*>(self->field_0008) + 728, nullptr)) &&
            sub_48D030(
                CSF_AT(void*,
                    CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14),
                    0x204),
                nullptr, 2))
        {
            const char fireMode = sub_47BBA0(
                reinterpret_cast<void*>(self->field_0008), nullptr);
            targetName = AiCore_GetTaskTargetNameBuffer(
                reinterpret_cast<void*>(self->field_0008), nullptr);
            return sub_47CAB0(
                reinterpret_cast<void*>(self->field_0008), nullptr,
                reinterpret_cast<int>(targetName + 16), fireMode);
        }
    }

    return sub_47CBE0(reinterpret_cast<void*>(self->field_0008), nullptr);
}

HOOK(0x0047D770, TCharTask_UpdateAttack, "auto");
