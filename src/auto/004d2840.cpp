#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* , char a2));
CSF_FUNC(int, __fastcall, C_00833ee8_DispatchByObjectType, 0x00424ED0, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, SceneLoader_DispatchFocusEntryByState, 0x00424F50, (void* self, void* , int* arg0));
CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionY, 0x004D1B50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ba40_SyncPlayerState, 0x004D1D50, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, C_0083ba40_UpdatePlayerState, 0x004D2300, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(double, __fastcall, NetCogibleDDL_GetWrappedAngleDegrees, 0x00740AA0, (void* self, void*));
CSF_FUNC(double, __fastcall, NetCogible_ReadAngleDegreesMod360, 0x00740AF0, (void* self, void*));
CSF_FUNC(double, __fastcall, NetCogible_ReadYawDegreesMod360, 0x00740B40, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpStats_GetPlayerExclusionFlag, 0x007411A0, (CMenuMpStats* self, void*));

static void __fastcall C_0083ba40_UpdatePlayerFocus(
    C_0083ba40* self, void* /*edx*/, CMenuMpStats* player, float elapsedTime)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* focusObject = CSF_AT(void*, owner, 0x30);

    alignas(8) uint8_t positionBytes[0x10];
    float* position = reinterpret_cast<float*>(positionBytes);

    position[3] = static_cast<float>(GetPlayerPositionZ(player, nullptr));
    float positionY = static_cast<float>(GetPlayerPositionY(player, nullptr));
    position[0] = static_cast<float>(GetPlayerPositionX(player, nullptr));

    unsigned enabled = (~CSF_AT(uint8_t, player, 0x452)) & 1;
    position[1] = positionY;
    position[2] = position[3];

    owner = reinterpret_cast<void*>(self->field_0014);
    C_0082dd78_StoreByteAndSetFlag(
        CSF_AT(C_0082dd78*, owner, 0x4C), nullptr,
        static_cast<char>(enabled));

    if (static_cast<uint8_t>(
            CMenuMpStats_GetPlayerExclusionFlag(player, nullptr)) == 0)
    {
        SceneLoader_DispatchFocusEntryByState(
            focusObject, nullptr, reinterpret_cast<int*>(positionBytes));
    }

    if (self->field_0041)
    {
        float yaw = static_cast<float>(
            NetCogible_ReadYawDegreesMod360(player, nullptr));
        float wrappedAngle = static_cast<float>(
            NetCogibleDDL_GetWrappedAngleDegrees(player, nullptr));
        float angle = static_cast<float>(
            NetCogible_ReadAngleDegreesMod360(player, nullptr));

        int yawBits;
        int wrappedAngleBits;
        int angleBits;
        std::memcpy(&yawBits, &yaw, sizeof(yawBits));
        std::memcpy(&wrappedAngleBits, &wrappedAngle, sizeof(wrappedAngleBits));
        std::memcpy(&angleBits, &angle, sizeof(angleBits));

        C_00833ee8_DispatchByObjectType(
            focusObject, nullptr, angleBits, wrappedAngleBits, yawBits);
    }
    else if ((CSF_AT(uint8_t, player, 0x453) & 1) == 0)
    {
        C_0083ba40_SyncPlayerState(self, nullptr, player);
        C_0083ba40_UpdatePlayerState(self, nullptr, player, elapsedTime);
    }
}

HOOK(0x004D2840, C_0083ba40_UpdatePlayerFocus, "auto");
