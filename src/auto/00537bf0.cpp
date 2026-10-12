#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, C_00830578_Construct, 0x0042C5E0, (C_00830578* self, void* , int menuArgument));
static CSF_FUNC(void*, __fastcall, MenuProgressInput_ResetSlotsAndUpdate, 0x0045BD10, (void* self, void*));
static CSF_FUNC(void*, __fastcall, C_00843720_InitializeMembers, 0x00537950, (int a1, int a2));

static int __fastcall C_00843720_Construct(
    C_00843720* self, void* /*edx*/, int ownerArgument, int menuArgument)
{
    C_00830578_Construct(
        reinterpret_cast<C_00830578*>(self), nullptr, menuArgument);

    CSF_AT(void**, self, 0x0000) = CSF_VTBL(0x00842038);
    CSF_AT(void**, self, 0x0004) = CSF_VTBL(0x00841FE8);
    CSF_AT(void**, self, 0x0010) = CSF_VTBL(0x00841FE4);

    CSF_AT(uint32_t, self, 0x0100) = 0;
    CSF_AT(uint32_t, self, 0x0104) = 0;
    CSF_AT(uint8_t, self, 0x0108) = 0;
    CSF_AT(uint8_t, self, 0x0109) = 0;
    CSF_AT(uint32_t, self, 0x010C) = 0;
    CSF_AT(uint8_t, self, 0x0110) = 1;
    CSF_AT(uint8_t, self, 0x0111) = 0;
    CSF_AT(uint8_t, self, 0x0112) = 0;
    CSF_AT(uint32_t, self, 0x0128) = 0;
    CSF_AT(uint32_t, self, 0x012C) = 15;
    CSF_AT(uint8_t, self, 0x0118) = 0;
    CSF_AT(uint32_t, self, 0x0154) = 0;
    CSF_AT(uint8_t, self, 0x0158) = 0;
    CSF_AT(uint32_t, self, 0x0178) = 15;
    CSF_AT(uint32_t, self, 0x0174) = 0;
    CSF_AT(uint8_t, self, 0x0164) = 0;

    MenuProgressInput_ResetSlotsAndUpdate(
        reinterpret_cast<uint8_t*>(self) + 0x018C, nullptr);

    CSF_AT(uint8_t, self, 0x0270) = 0;
    CSF_AT(int, self, 0x00F8) = ownerArgument;
    CSF_AT(uint32_t, self, 0x013C) = 0;
    CSF_AT(uint32_t, self, 0x014C) = 0;
    CSF_AT(uint32_t, self, 0x0138) = 0;
    CSF_AT(uint8_t, self, 0x0187) = 0;
    CSF_AT(int, self, 0x0144) = -1;

    C_00843720_InitializeMembers(reinterpret_cast<int>(self), 0);

    CSF_AT(uint8_t, self, 0x0180) = 1;
    return reinterpret_cast<int>(self);
}

HOOK(0x00537BF0, C_00843720_Construct, "auto");
