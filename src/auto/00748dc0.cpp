#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateAuthoritativeState, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateReplicaState, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(int64_t, __fastcall, NetCogibleDDL_UpdateModeThree, 0x00748500, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateModeOne, 0x00748BF0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, S_00747f20_UpdateStateAndCommit, 0x00748D30, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_74b150_SetFlag8, 0x0074B150, (void* self, void*));
CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));
CSF_FUNC(char, __fastcall, C_0074b2a0_SetFlag, 0x0074B2A0, (void* self, void*));
CSF_FUNC(char, __fastcall, C_751990_SetFlagAndReturnTrue, 0x00751990, (void* self, void*));

static char __fastcall NetCogibleDDL_UpdateStateAndTrackChanges(
    void* self, void* /*edx*/, int updateArgument)
{
    auto* state = static_cast<uint8_t*>(self);
    auto* owner = state - 0x308;

    CSF_AT(uint32_t, state, 0x0C) = 2;
    if (static_cast<uint8_t>(Sub_64B250(owner, nullptr)) == 0)
        return NetCogibleDDL_UpdateReplicaState(owner, nullptr);

    switch (CSF_AT(int8_t, state, -0x2AC))
    {
    case 1:
    {
        float elapsedTime;
        std::memcpy(&elapsedTime, &updateArgument, sizeof(elapsedTime));
        NetCogibleDDL_UpdateModeOne(owner, nullptr, elapsedTime);
        break;
    }
    case 2:
        S_00747f20_UpdateStateAndCommit(owner, nullptr, updateArgument);
        break;
    case 3:
        NetCogibleDDL_UpdateModeThree(owner, nullptr, updateArgument);
        break;
    }

    {
        const uint8_t value = CSF_AT(uint8_t, state, -0x2AC);
        const bool changed = value != CSF_AT(uint8_t, state, -0x2AA);
        CSF_AT(uint8_t, state, -0x2AA) = value;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(state - 0x2AC, nullptr);
    }

    {
        const uint32_t first = CSF_AT(uint32_t, state, -0x20);
        const uint32_t previousFirst = CSF_AT(uint32_t, state, -0x10);
        const uint32_t second = CSF_AT(uint32_t, state, -0x1C);
        const bool changed =
            first != previousFirst || second != CSF_AT(uint32_t, state, -0x0C);
        CSF_AT(uint32_t, state, -0x0C) = second;
        CSF_AT(uint32_t, state, -0x10) = first;
        if (changed)
            C_74b150_SetFlag8(state - 0x20, nullptr);
    }

    {
        const uint8_t value = CSF_AT(uint8_t, state, -0x08);
        const bool changed = value != CSF_AT(uint8_t, state, -0x06);
        CSF_AT(uint8_t, state, -0x06) = value;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(state - 0x08, nullptr);
    }

    {
        const uint16_t value = CSF_AT(uint16_t, state, -0x32);
        const bool changed = value != CSF_AT(uint16_t, state, -0x2E);
        CSF_AT(uint16_t, state, -0x2E) = value;
        if (changed)
            C_751990_SetFlagAndReturnTrue(state - 0x32, nullptr);
    }

    {
        const uint16_t value = CSF_AT(uint16_t, state, -0x2C);
        const bool changed = value != CSF_AT(uint16_t, state, -0x28);
        CSF_AT(uint16_t, state, -0x28) = value;
        if (changed)
            C_751990_SetFlagAndReturnTrue(state - 0x2C, nullptr);
    }

    {
        const uint16_t value = CSF_AT(uint16_t, state, -0x44);
        const bool changed = value != CSF_AT(uint16_t, state, -0x40);
        CSF_AT(uint16_t, state, -0x40) = value;
        if (changed)
            C_751990_SetFlagAndReturnTrue(state - 0x44, nullptr);
    }

    {
        const uint32_t value = CSF_AT(uint32_t, state, -0x2C4);
        const bool changed = value != CSF_AT(uint32_t, state, -0x2BC);
        CSF_AT(uint32_t, state, -0x2BC) = value;
        if (changed)
            C_0074b2a0_SetFlag(state - 0x2C4, nullptr);
    }

    {
        const uint32_t value = CSF_AT(uint32_t, state, -0x2B8);
        const bool changed = value != CSF_AT(uint32_t, state, -0x2B0);
        CSF_AT(uint32_t, state, -0x2B0) = value;
        if (changed)
            C_0074b2a0_SetFlag(state - 0x2B8, nullptr);
    }

    {
        const uint32_t value = CSF_AT(uint32_t, state, -0x50);
        const bool changed = value != CSF_AT(uint32_t, state, -0x48);
        CSF_AT(uint32_t, state, -0x48) = value;
        if (changed)
            C_0074b2a0_SetFlag(state - 0x50, nullptr);
    }

    {
        const uint8_t value = CSF_AT(uint8_t, state, -0x2A9);
        const bool changed = value != CSF_AT(uint8_t, state, -0x2A7);
        CSF_AT(uint8_t, state, -0x2A7) = value;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(state - 0x2A9, nullptr);
    }

    return NetCogibleDDL_UpdateAuthoritativeState(owner, nullptr);
}

HOOK(0x00748DC0, NetCogibleDDL_UpdateStateAndTrackChanges, "auto");
