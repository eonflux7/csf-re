#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (float* self, void*));
CSF_FUNC(double, __fastcall, NetCogibleDDL_GetWrappedAngleDegrees, 0x00740AA0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083BA20);

static char __fastcall UpdatePlayerRelativeAnimation(
    void* self, void* /*edx*/, CMenuMpStats* player, char force)
{
    float playerZ = static_cast<float>(GetPlayerPositionZ(player, nullptr));
    float direction[2];
    direction[0] = static_cast<float>(GetPlayerPositionX(player, nullptr));

    const float angle = static_cast<float>(
        NetCogibleDDL_GetWrappedAngleDegrees(player, nullptr));
    const float facingX = static_cast<float>(std::sin(
        NetCogibleDDL_GetWrappedAngleDegrees(player, nullptr) * g_DegreesToRadians));
    const float facingZ = static_cast<float>(
        std::cos(angle * g_DegreesToRadians));

    direction[0] -= CSF_AT(float, self, 0x4C);
    direction[1] = playerZ - CSF_AT(float, self, 0x54);

    float* normalized = NormalizeVector2(direction, nullptr);
    playerZ = normalized[1];
    const float directionX = normalized[0];
    const bool inFront =
        playerZ * facingZ + directionX * facingX > g_fZero;

    int state;
    int animation;
    if (CSF_AT(uint8_t, self, 0x65))
    {
        state = 4;
        animation = 7;
    }
    else if (CSF_AT(uint8_t, self, 0x66))
    {
        state = 3;
        animation = 5;
    }
    else if (inFront)
    {
        state = 1;
        animation = 1;
    }
    else
    {
        state = 2;
        animation = 3;
    }

    if (CSF_AT(int, self, 0x58) == state && force == 0)
        return force;

    void* animatedObject = CSF_AT(void*, self, 0x14);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, animatedObject, 0)[0x8C / 4])(
                animatedObject, nullptr, animation, 0, -1, 0);

    CSF_AT(int, self, 0x58) = state;
    return static_cast<char>(result);
}

HOOK(0x004D26E0, UpdatePlayerRelativeAnimation, "auto");
