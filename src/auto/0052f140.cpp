#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __stdcall, MenuManager_SetGameplayControllerState, 0x0042DF80, (int a1));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __fastcall, MpSpawn_SetCurrentSpawnIndex, 0x005514E0, (void* self, void* , int spawnIndex));
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetSpawnValue, 0x005515E0, (void* self, void* , int value));

static int __fastcall CMenuMpLoadout_ApplySelectionAndStartGameplay(
    CMenuMpLoadout* self, void* /*edx*/)
{
    const int spawnIndex = static_cast<int32_t>(self->field_0040);
    if (spawnIndex == -1 || !self->field_0044 ||
        self->field_0048 == static_cast<uint32_t>(-1))
    {
        return spawnIndex;
    }

    void* spawnState = reinterpret_cast<void*>(GetGlobal8B0688());
    MpSpawn_SetCurrentSpawnIndex(spawnState, nullptr, spawnIndex);

    if (!self->field_0040)
    {
        void* state = reinterpret_cast<void*>(GetGlobal8B0688());
        CSF_AT(uint8_t, state, 0x1401) = 1;
        CSF_AT(uint8_t, state, 0x1402) = 0;
    }
    else
    {
        switch (self->field_0044)
        {
        case 1u:
            MpSpawnPoints_SetSpawnValue(
                reinterpret_cast<void*>(GetGlobal8B0688()), nullptr, 0);
            break;
        case 2u:
            MpSpawnPoints_SetSpawnValue(
                reinterpret_cast<void*>(GetGlobal8B0688()), nullptr, 1);
            break;
        case 3u:
            MpSpawnPoints_SetSpawnValue(
                reinterpret_cast<void*>(GetGlobal8B0688()), nullptr, 2);
            break;
        }

        void* definitions = reinterpret_cast<void*>(GetValue8B0554());
        void* weaponList = CSF_AT(void*, definitions, 0x1FC0);
        const uint32_t weaponIndex = self->field_0048;
        const int selectedWeapon =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, weaponList, 0)[0x0C / 4])(
                weaponList, nullptr, weaponIndex);

        void* state = reinterpret_cast<void*>(GetGlobal8B0688());
        CSF_AT(int, state, 0x13D4) = selectedWeapon;
    }

    void* menuOwner = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*, CMenuMpLoadout*, int)>(
        CSF_AT(void**, menuOwner, 0)[0x60 / 4])(
        menuOwner, nullptr, self, 0);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* child = CSF_AT(void*, globalState, 0x10);
    if (child)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, child, 0)[0x24 / 4])(child, nullptr);
    }

    GetGlobal8afc78();
    MenuManager_SetGameplayControllerState(1);
    return MenuSystem_ToggleChildFlag(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
}

HOOK(0x0052F140, CMenuMpLoadout_ApplySelectionAndStartGameplay, "auto");
