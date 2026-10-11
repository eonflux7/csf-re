#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, C_008316d8_GetTransitionScale, 0x00476B80, (C_008319c0* self, void*));
CSF_FUNC(void, __fastcall, MpSpawnPoints_CreateSpawn, 0x0048C370, (void* self, void* edx, uint32_t* position, uint32_t* direction, int count, float spread, uint32_t type, void* parameters, int direct));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_WeaponDegreesToRadians, 0x008316AC);
CSF_GLOBAL(float, g_WeaponTransitionThreshold, 0x008316B0);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_008316d8_SpawnProjectiles(
    C_008316d8* self, void* /*edx*/,
    uint32_t positionX, uint32_t positionY, uint32_t positionZ,
    uint32_t directionX, uint32_t directionY, uint32_t directionZ,
    int count, void* parameters)
{
    const uint8_t transitionEnabled = self->field_0016;
    float spreadAngle =
        CSF_AT(float, reinterpret_cast<void*>(self->field_0040), 0x120)
        * g_WeaponDegreesToRadians;

    if (transitionEnabled)
    {
        void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        if (CSF_AT(float, globalValue, 0xAF4) > g_WeaponTransitionThreshold)
        {
            spreadAngle = static_cast<float>(
                C_008316d8_GetTransitionScale(
                    reinterpret_cast<C_008319c0*>(self), nullptr)
                * spreadAngle);
        }
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    const int variation = GetStateValue(state, nullptr, 100) - 50;
    const float spread = static_cast<float>(variation) * g_ProgressScale * spreadAngle;

    void* spawnManager = CSF_AT(void*, g_pCurrentScene, 0xA8);
    const int direct =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            self->vtbl[0xCC / 4])(self, nullptr);
    const uint32_t projectileType = self->field_000c;

    uint32_t position[3] = {positionX, positionY, positionZ};
    uint32_t direction[3] = {directionX, directionY, directionZ};
    MpSpawnPoints_CreateSpawn(
        spawnManager, nullptr, position, direction, count,
        spread, projectileType, parameters, direct);
}

HOOK(0x00476D50, C_008316d8_SpawnProjectiles, "auto");
