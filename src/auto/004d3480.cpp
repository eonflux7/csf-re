#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* , char a2));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionY, 0x004D1B50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083ba40_UpdatePlayerBinding, 0x004D21E0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_0083ba40_UpdatePlayerMovement, 0x004D2840, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(void, __fastcall, C_0083ba40_UpdatePlayerAnimation, 0x004D2C40, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(int, __fastcall, C_0083ba40_InitializePlayerState, 0x004D31E0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, C_0083ba40_RefreshPlayerState, 0x004D3360, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, CMenuMpStats_GetPlayerExclusionFlag, 0x007411A0, (CMenuMpStats* self, void*));

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083ba40_UpdatePlayerState(
    C_0083ba40* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_0041)
    {
        auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
        void* controller = reinterpret_cast<void*>(owner->vGetField016c());
        void* resources =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0x68 / 4])(
                    controller, nullptr);

        C_0083ba40_InitializePlayerState(self, nullptr);

        self->field_00a8 =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resources, 0)[0x68 / 4])(
                    resources, nullptr, 20);
        self->field_00ac =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resources, 0)[0x68 / 4])(
                    resources, nullptr, 30);
        self->field_00b0 =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resources, 0)[0x68 / 4])(
                    resources, nullptr, 10);
        self->field_00b4 =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, resources, 0)[0x68 / 4])(
                    resources, nullptr, 1);

        reinterpret_cast<C_00833908*>(self->field_0014)->field_0078 = 1;
    }

    const int enabled = GetGlobalDword_73C020();
    if (!enabled)
        return static_cast<char>(enabled);

    if (!self->field_0040)
    {
        auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(uint32_t, owner, 0x18))
            self->field_0040 = 1;
        if (!self->field_0040)
            return static_cast<char>(self->field_0040);
    }

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
    void* playerInterface = CSF_AT(void*, owner, 0x18);
    if (!playerInterface)
    {
        return static_cast<char>(CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(&owner->field_001c)));
    }

    auto* player = reinterpret_cast<CMenuMpStats*>(
        reinterpret_cast<uint8_t*>(playerInterface) - 1712);
    void* embeddedInterface = reinterpret_cast<uint8_t*>(player) + 1712;

    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, embeddedInterface, 0)[0x0C / 4])(
            embeddedInterface, nullptr, elapsedTime);

    owner = reinterpret_cast<C_00833908*>(self->field_0014);
    playerInterface = CSF_AT(void*, owner, 0x18);
    const char finished =
        reinterpret_cast<char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, playerInterface, 0)[0x08 / 4])(
                playerInterface, nullptr);
    if (finished)
        return finished;

    owner = reinterpret_cast<C_00833908*>(self->field_0014);
    void* controller = reinterpret_cast<void*>(owner->vGetField016c());
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x68 / 4])(
            controller, nullptr);

    alignas(8) uint8_t positionBytes[0x10];
    float* position = reinterpret_cast<float*>(positionBytes);

    if (static_cast<uint8_t>(
            CMenuMpStats_GetPlayerExclusionFlag(player, nullptr)))
    {
        owner = reinterpret_cast<C_00833908*>(self->field_0014);
        auto* positionTarget =
            reinterpret_cast<C_0082dd78*>(owner->field_004c);
        C_0082dd78_StoreByteAndSetFlag(positionTarget, nullptr, 1);

        owner = reinterpret_cast<C_00833908*>(self->field_0014);
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;

        const float* objectPosition = GetObjectPositionVector(owner, nullptr);
        position[0] = objectPosition[0];
        position[1] = objectPosition[1];
        position[2] = objectPosition[2];
        position[1] -= elapsedTime * g_Const200_0f;

        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, positionTarget, 0)[0x50 / 4])(
                positionTarget, nullptr, position);
    }
    else
    {
        C_0083ba40_RefreshPlayerState(
            self, nullptr, reinterpret_cast<unsigned int>(player));
        C_0083ba40_UpdatePlayerMovement(
            self, nullptr, player, elapsedTime);
    }

    C_0083ba40_UpdatePlayerAnimation(
        self, nullptr, player, elapsedTime);

    // This callee's float parameter receives the player's pointer bits.
    const uint32_t playerBits = reinterpret_cast<uint32_t>(player);
    float playerArgument;
    std::memcpy(&playerArgument, &playerBits, sizeof(playerArgument));
    C_0083ba40_UpdatePlayerBinding(self, nullptr, playerArgument);

    position[3] = static_cast<float>(GetPlayerPositionZ(player, nullptr));
    const float playerY = static_cast<float>(
        GetPlayerPositionY(player, nullptr));
    position[0] = static_cast<float>(GetPlayerPositionX(player, nullptr));

    uint32_t playerXBits;
    std::memcpy(&playerXBits, &position[0], sizeof(playerXBits));
    self->field_004c = playerXBits;
    std::memcpy(&self->field_0050, &playerY, sizeof(playerY));
    position[1] = playerY;
    position[2] = position[3];
    std::memcpy(&CSF_AT(uint32_t, self, 0x54),
                &position[2], sizeof(float));

    return static_cast<char>(playerXBits);
}

HOOK(0x004D3480, C_0083ba40_UpdatePlayerState, "auto");
