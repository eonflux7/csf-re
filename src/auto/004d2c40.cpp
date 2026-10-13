#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

CSF_FUNC(int, __fastcall, C_00833908_AddSceneObjectFlags, 0x0048E6D0, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, QueryNestedChainValue20, 0x004A67A0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetNestedValueFor20, 0x004A67C0, (void* self, void*));
CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ba40_HandleDisabledInput, 0x004D1F40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083ba40_HandleExcludedPlayer, 0x004D1F80, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_0083ba40_HandlePrimaryAction, 0x004D2020, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083ba40_HandleSecondaryAction, 0x004D2050, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083ba40_UpdatePlayerInput, 0x004D2070, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_0083ba40_UpdateMovement, 0x004D26E0, (void* self, void* edx, void* a2, char a3));
CSF_FUNC(void, __fastcall, C_0083ba40_ResetDelayedAction, 0x004D2A20, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083ba40_UpdateHeldAction, 0x004D2B40, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, CMenuMpStats_GetPlayerExclusionFlag, 0x007411A0, (CMenuMpStats* self, void*));

static int __fastcall C_00833908_ForwardControlRequest(
    C_00833908* self, void* /*edx*/, int request, int argument, int index, int flags)
{
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        self->vtbl[0x8C / 4])(self, nullptr, request, argument, index, flags);
}

static void __fastcall C_0083ba40_UpdateControls(
    C_0083ba40* self, void* /*edx*/, CMenuMpStats* player, float deltaTime)
{
    if (self->field_009c > g_fZero)
    {
        const float remaining = self->field_009c - deltaTime;
        self->field_009c = remaining;
        if (remaining < g_fZero)
        {
            void* chain = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0048);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, chain, 0)[0x28 / 4])(chain, nullptr);

            C_00833908* owner = reinterpret_cast<C_00833908*>(self->field_0014);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                owner->vtbl[0xB4 / 4])(owner, nullptr, 0);

            C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 0, 0, -1, 0);
            self->field_0060 = 0;
            C_0083ba40_ResetDelayedAction(self, nullptr);
            if (self->field_0061)
            {
                C_00833908_AddSceneObjectFlags(
                    reinterpret_cast<C_00833908*>(self->field_0014), nullptr);
                self->field_0061 = 0;
            }
        }
        return;
    }

    if (self->field_0041)
    {
        C_00833908_ForwardControlRequest(
            reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 0, 0, -1, 0);
        self->field_0041 = 0;
        return;
    }

    const uint32_t inputFlags = CSF_AT(uint16_t, player, 0x452);
    const float positionZ = static_cast<float>(GetPlayerPositionZ(player, nullptr));
    const double positionX = GetPlayerPositionX(player, nullptr);
    const double offsetX = *reinterpret_cast<float*>(&self->field_004c) - positionX;
    const float offsetZ = *reinterpret_cast<float*>(&self->field_0054) - positionZ;
    const bool moved = offsetX * offsetX + offsetZ * offsetZ > g_ControlHoldThreshold;
    const char excluded = static_cast<char>(CMenuMpStats_GetPlayerExclusionFlag(player, nullptr));
    const uint8_t stance = (inputFlags & 8) != 0;
    reinterpret_cast<C_00833908*>(self->field_0014)->field_0078 =
        CSF_AT(uint8_t, player, 0x443) != 1;

    if ((inputFlags & 0x100) != 0)
    {
        C_0083ba40_HandleDisabledInput(self, nullptr);
        goto Finish;
    }
    if (excluded)
    {
        C_0083ba40_HandleExcludedPlayer(self, nullptr, deltaTime);
        goto Finish;
    }

    C_0083ba40_UpdatePlayerInput(self, nullptr, player);
    if ((inputFlags & 0x800) != 0)
    {
        if (self->field_005c != 1)
            self->field_005c = C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 20, 0, -1, 0) != -1;
        goto Finish;
    }
    if ((inputFlags & 0x200) != 0 && (self->field_006c & 0x200) == 0)
    {
        const uint32_t action = self->field_005c;
        if (action != 2)
        {
            if (action)
                QueryNestedChainValue20(reinterpret_cast<void*>(
                    reinterpret_cast<C_00833908*>(self->field_0014)->field_0048), nullptr);
            self->field_005c = C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 19, 0, -1, 0) != -1 ? 2 : 0;
            goto Finish;
        }
    }
    if ((inputFlags & 0x1000) != 0 && (self->field_006c & 0x1000) == 0)
    {
        const uint32_t action = self->field_005c;
        if (action != 3)
        {
            if (action)
                QueryNestedChainValue20(reinterpret_cast<void*>(
                    reinterpret_cast<C_00833908*>(self->field_0014)->field_0048), nullptr);
            self->field_005c = C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 21, 0, -1, 0) != -1 ? 3 : 0;
            goto Finish;
        }
    }
    if ((inputFlags & 0x2000) != 0)
    {
        const uint32_t action = self->field_005c;
        if (action != 4)
        {
            if (action)
                QueryNestedChainValue20(reinterpret_cast<void*>(
                    reinterpret_cast<C_00833908*>(self->field_0014)->field_0048), nullptr);
            self->field_005c = C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 22, 0, -1, 0) != -1 ? 4 : 0;
            goto Finish;
        }
    }
    if ((inputFlags & 0x8000) != 0)
    {
        const uint32_t action = self->field_005c;
        if (action != 5)
        {
            if (action)
                QueryNestedChainValue20(reinterpret_cast<void*>(
                    reinterpret_cast<C_00833908*>(self->field_0014)->field_0048), nullptr);
            self->field_005c = C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 23, 0, -1, 0) != -1 ? 5 : 0;
            goto Finish;
        }
    }

    {
        const uint8_t previousEquipmentState = self->field_0067;
        const bool equipmentEnabled = (inputFlags & 0x4000) != 0;
        if (equipmentEnabled ? !previousEquipmentState : previousEquipmentState != 0)
        {
            const int enabled = equipmentEnabled ? 1 : 0;
            void* equipment = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0044);
            const int itemIndex = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, equipment, 0)[0xBC / 4])(equipment, nullptr, 22);

            equipment = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0044);
            void* item = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, equipment, 0)[0xC8 / 4])(equipment, nullptr, itemIndex);
            void* itemState = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, item, 0)[0xA8 / 4])(item, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, itemState, 0)[0x34 / 4])(itemState, nullptr, enabled);

            equipment = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0044);
            item = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, equipment, 0)[0xC8 / 4])(equipment, nullptr, itemIndex);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, item, 0)[0x30 / 4])(item, nullptr, enabled);
            self->field_0067 = enabled;
        }

        if ((inputFlags & 1) != 0)
        {
            C_0083ba40_HandlePrimaryAction(self, nullptr);
            goto Finish;
        }

        bool stanceChanged = false;
        if (self->field_0042 != stance)
        {
            C_00833908* owner = reinterpret_cast<C_00833908*>(self->field_0014);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                owner->vtbl[0xB4 / 4])(owner, nullptr, stance);
            stanceChanged = self->field_0042 != stance;
        }
        const char movementChanged = stanceChanged || self->field_0084;
        self->field_0042 = stance;

        if (moved)
        {
            C_0083ba40_UpdateMovement(self, nullptr, player, movementChanged);
        }
        else if (self->field_0058 || movementChanged)
        {
            C_00833908_ForwardControlRequest(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr, 0, 0, -1, 0);
            self->field_0058 = 0;
        }

        uint8_t heldAction = 0;
        if ((inputFlags & 0x80) != 0)
        {
            if (!self->field_0043)
                self->field_0098 = 0;
            C_0083ba40_UpdateHeldAction(self, nullptr, deltaTime);
            if (!self->field_0048 && *reinterpret_cast<float*>(&self->field_0044) > g_fZero)
                heldAction = 1;
        }
        else if ((inputFlags & 0x400) != 0)
        {
            C_0083ba40_HandleSecondaryAction(self, nullptr);
        }
        else if ((self->field_0043 || self->field_0048) && !self->field_005c)
        {
            QueryNestedChainValue20(reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0048), nullptr);
        }

        self->field_0043 = heldAction;
        self->field_0048 = (inputFlags & 0x400) != 0 && !heldAction;
        if (*reinterpret_cast<float*>(&self->field_0044) >= g_fZero)
            *reinterpret_cast<float*>(&self->field_0044) =
                *reinterpret_cast<float*>(&self->field_0044) - deltaTime;
    }

Finish:
    {
        const uint8_t previousVisibilityState = self->field_0068;
        const bool hideItem = self->field_005c == 4;
        if (hideItem ? !previousVisibilityState : previousVisibilityState != 0)
        {
            void* equipment = reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0044);
            void* item = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, equipment, 0)[0xD8 / 4])(equipment, nullptr);
            if (item && reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, item, 0)[0xA8 / 4])(item, nullptr))
            {
                void* itemState = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, item, 0)[0xA8 / 4])(item, nullptr);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, itemState, 0)[0x3C / 4])(itemState, nullptr, hideItem ? 0 : 1);
            }
            self->field_0068 = hideItem ? 1 : 0;
        }
        const uint32_t action = self->field_005c;
        if (action && action <= 5 &&
            static_cast<uint8_t>(GetNestedValueFor20(reinterpret_cast<void*>(
                reinterpret_cast<C_00833908*>(self->field_0014)->field_0048), nullptr)))
        {
            self->field_005c = 0;
        }
        self->field_006c = inputFlags;
    }
}

HOOK(0x004D2C40, C_0083ba40_UpdateControls, "auto");
