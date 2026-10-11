#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall WeaponFirearms_SetWeaponActionMode(
    void* self, void* /*edx*/, std::uint8_t alternateMode)
{
    const int handleState = CSF_AT(int, self, 0x6C);
    if (handleState < 0)
        return handleState;

    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x64));
    if (!resolved)
        return resolved;

    void* weaponOwner = reinterpret_cast<void*>(resolved);
    void* weapon = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, weaponOwner, 0)[0x28 / 4])(weaponOwner, nullptr);

    if (((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, weapon, 0)[0xA8 / 4])(weapon, nullptr))
    {
        weapon = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, weaponOwner, 0)[0x28 / 4])(weaponOwner, nullptr);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, weapon, 0)[0xA4 / 4])(weapon, nullptr);
    }

    weapon = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, weaponOwner, 0)[0x28 / 4])(weaponOwner, nullptr);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, weapon, 0)[0x3C / 4])(weapon, nullptr, 1);

    const bool isPrimary = CSF_AT(std::uint32_t,
        CSF_AT(void*, self, 0x40), 0x150) == 1;
    const int actionMode = alternateMode
        ? (isPrimary ? 40 : 50)
        : (isPrimary ? 50 : 40);

    void* character = CSF_AT(void*, self, 0x04);
    void* actionController = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
    void** controllerVtable = CSF_AT(void**, actionController, 0);

    weapon = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, weaponOwner, 0)[0x28 / 4])(weaponOwner, nullptr);

    return ((int(__fastcall*)(void*, void*, void*, int, int))
        controllerVtable[0xA0 / 4])(
            actionController, nullptr, weapon, actionMode, 0);
}

HOOK(0x004782E0, WeaponFirearms_SetWeaponActionMode, "auto");
