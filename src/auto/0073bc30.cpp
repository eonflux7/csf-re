#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateAuthoritativeState, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateReplicaState, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_74b150_SetFlag8, 0x0074B150, (void* self, void*));
CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));
CSF_FUNC(char, __fastcall, C_0074b2a0_SetFlag, 0x0074B2A0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall CMenuMpStats_UpdateReplicatedState(
    uint8_t* self, void* /*edx*/, float elapsedTime)
{
    void* owner = self - 0x80;
    CSF_AT(int, self, 0x0C) = 14;

    if (static_cast<uint8_t>(Sub_64B250(owner, nullptr)) != 0)
    {
        uint32_t previousValue = CSF_AT(uint32_t, self, -0x1C);
        uint32_t currentValue = CSF_AT(uint32_t, self, -0x24);
        bool changed = currentValue != previousValue;
        CSF_AT(uint32_t, self, -0x1C) = currentValue;
        if (changed)
            C_0074b2a0_SetFlag(self - 0x24, nullptr);

        uint8_t previousByte = CSF_AT(uint8_t, self, -0x34);
        uint8_t currentByte = CSF_AT(uint8_t, self, -0x36);
        changed = currentByte != previousByte;
        CSF_AT(uint8_t, self, -0x34) = currentByte;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(self - 0x36, nullptr);

        previousByte = CSF_AT(uint8_t, self, -0x3A);
        currentByte = CSF_AT(uint8_t, self, -0x3C);
        changed = currentByte != previousByte;
        CSF_AT(uint8_t, self, -0x3A) = currentByte;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(self - 0x3C, nullptr);

        uint32_t firstValue = CSF_AT(uint32_t, self, -0x18);
        uint32_t previousFirstValue = CSF_AT(uint32_t, self, -0x08);
        uint32_t secondValue = CSF_AT(uint32_t, self, -0x14);
        changed = firstValue != previousFirstValue ||
                  secondValue != CSF_AT(uint32_t, self, -0x04);
        CSF_AT(uint32_t, self, -0x04) = secondValue;
        CSF_AT(uint32_t, self, -0x08) = firstValue;
        if (changed)
            C_74b150_SetFlag8(self - 0x18, nullptr);

        previousByte = CSF_AT(uint8_t, self, -0x37);
        currentByte = CSF_AT(uint8_t, self, -0x39);
        changed = currentByte != previousByte;
        CSF_AT(uint8_t, self, -0x37) = currentByte;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(self - 0x39, nullptr);
    }

    if (CSF_AT(float, self, 0x1C) >= g_fZero)
        CSF_AT(float, self, 0x1C) =
            CSF_AT(float, self, 0x1C) - elapsedTime;

    if (static_cast<uint8_t>(Sub_64B250(owner, nullptr)) != 0)
        return NetCogibleDDL_UpdateAuthoritativeState(owner, nullptr);

    return NetCogibleDDL_UpdateReplicaState(owner, nullptr);
}

HOOK(0x0073BC30, CMenuMpStats_UpdateReplicatedState, "auto");
