#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_ProjectilePositionScale, 0x008330E4);

static char __fastcall CCharWeaponFsm_LaunchProjectile(CCharWeaponFsm* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* controller = CSF_AT(void*, owner, 0x48);
    if (controller &&
        ((bool(__fastcall*)(void*, void*, CCharWeaponFsm*))
            CSF_AT(void**, controller, 0)[0x10 / 4])(controller, nullptr, self))
    {
        owner = reinterpret_cast<void*>(self->field_0008);
        controller = CSF_AT(void*, owner, 0x48);
        ((void(__fastcall*)(void*, void*, CCharWeaponFsm*))
            CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr, self);
    }

    owner = reinterpret_cast<void*>(self->field_0008);

    // The matrix query receives all twelve bytes, including the saved owner.
    struct MatrixQuery
    {
        float* matrix;
        std::uint8_t destroy;
        std::uint8_t padding[3];
        void* owner;
    };
    MatrixQuery query{nullptr, 0, {0, 0, 0}, owner};

    bool useRequestedWeapon = false;
    if (static_cast<std::uint8_t>(IsField1BCNonNegative(owner, nullptr)) == 0)
    {
        void* currentOwner = reinterpret_cast<void*>(self->field_0008);
        if (CSF_AT(std::uint8_t, currentOwner, 0x1E4) &&
            CSF_AT(int, currentOwner, 0x1E8) != -1 &&
            CSF_AT(std::uint8_t, owner, 0x1E4) &&
            CSF_AT(int, owner, 0x1E8) != -1 &&
            CSF_AT(int, owner, 0x1EC) > 0)
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            useRequestedWeapon =
                GetStateValue(state, nullptr, 101) < CSF_AT(int, owner, 0x1EC);
        }
    }
    self->field_0071 = useRequestedWeapon;

    void* weaponHandle = reinterpret_cast<void*>(self->vSlotD8());
    if (!weaponHandle)
        return static_cast<char>(self->vSlot134(9, 0));

    void* weapon = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, weaponHandle, 0)[0xA8 / 4])(weaponHandle, nullptr);
    if (!weapon)
        return static_cast<char>(self->vSlot134(9, 0));

    if (static_cast<std::uint8_t>(IsField1BCNonNegative(
            reinterpret_cast<void*>(self->field_0008), nullptr)))
        return static_cast<char>(self->vSlot134(9, 0));

    void* projectile = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, weapon, 0)[0x78 / 4])(weapon, nullptr);
    if (!projectile)
        return static_cast<char>(self->vSlot134(9, 0));

    if (!((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, projectile, 0)[0x2C / 4])(projectile, nullptr))
        return static_cast<char>(self->vSlot134(9, 0));

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, weapon, 0)[0xA4 / 4])(weapon, nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, projectile, 0)[0x20 / 4])(projectile, nullptr);

    void* transformSource = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, weapon, 0)[0x18 / 4])(weapon, nullptr);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, transformSource, 0)[0x8C / 4])(
            transformSource, nullptr, &query);

    float position[3] = {
        query.matrix[4] * g_ProjectilePositionScale,
        query.matrix[5] * g_ProjectilePositionScale,
        query.matrix[6] * g_ProjectilePositionScale
    };
    ((void(__fastcall*)(void*, void*, float*, float*, int))
        CSF_AT(void**, projectile, 0)[0x44 / 4])(
            projectile, nullptr, position, query.matrix + 12, 0);

    bool setDefaultParameter = !self->field_0071;
    if (!setDefaultParameter)
    {
        void* gameState = reinterpret_cast<void*>(GetValue8B0554());
        void* weaponDefinitions = CSF_AT(void*, gameState, 0x1FC8);
        void* definition = ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, weaponDefinitions, 0)[0x8 / 4])(
                weaponDefinitions, nullptr, CSF_AT(int, query.owner, 0x1E8));

        gameState = reinterpret_cast<void*>(GetValue8B0554());
        void* weaponRegistry = CSF_AT(void*, gameState, 0x1FC0);
        int weaponIndex = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, weaponRegistry, 0)[0xC / 4])(
                weaponRegistry, nullptr, CSF_AT(int, definition, 0xC0));

        if (weaponIndex == -1)
        {
            setDefaultParameter = true;
        }
        else
        {
            gameState = reinterpret_cast<void*>(GetValue8B0554());
            weaponRegistry = CSF_AT(void*, gameState, 0x1FC0);
            void* requestedWeapon = ((void*(__fastcall*)(void*, void*, int))
                CSF_AT(void**, weaponRegistry, 0)[0x8 / 4])(
                    weaponRegistry, nullptr, weaponIndex);

            weaponHandle = reinterpret_cast<void*>(self->vSlotD8());
            void* currentWeapon = reinterpret_cast<void*>(
                Handle_GetValuePointer(weaponHandle, nullptr));
            setDefaultParameter =
                CSF_AT(std::uint32_t, currentWeapon, 0x74) !=
                CSF_AT(std::uint32_t, requestedWeapon, 0x74);
        }
    }

    if (setDefaultParameter)
    {
        void* parameter = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, weapon, 0)[0x50 / 4])(weapon, nullptr);
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, parameter, 0)[0x14 / 4])(parameter, nullptr, 3.0f);
    }

    if (query.destroy)
        return static_cast<char>(
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(query.matrix)));
    return static_cast<char>(query.destroy);
}

HOOK(0x00480330, CCharWeaponFsm_LaunchProjectile, "auto");
