#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, ResetPendingFlagAndGetInnerBlockAddress, 0x0044FFB0, (void* self, void*));
CSF_FUNC(int, __fastcall, ClearPendingFlagAndGetInnerBlockAddress, 0x0044FFD0, (void* self, void*));
CSF_FUNC(void, __fastcall, S_004bd310_ProcessFlagLevelBlock, 0x004C4630, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, S_004bd310_ProcessSlotBlock, 0x004C4690, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __fastcall, C_0083e9d0_CheckInnerBlockFlagLevel, 0x004E72C0, (C_0083e9d0* self, void* , int memberIndex, void* outBlock));
CSF_FUNC(bool, __fastcall, S_004f0cd0_ResolveSlotIntoOutput, 0x004F1530, (S_004f0cd0* self, void* , int a2, uint32_t* a3));

static int __fastcall S_004bd310_EvaluateCoverOptions(S_004bd310* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0000);
    self->field_000c = 0;

    C_0083e9d0* flagLevels = CSF_AT(C_0083e9d0*, owner, 0x3C);
    uint32_t* flagBlock = reinterpret_cast<uint32_t*>(
        ResetPendingFlagAndGetInnerBlockAddress(g_pCurrentScene, nullptr));

    if (C_0083e9d0_CheckInnerBlockFlagLevel(flagLevels, nullptr, 3, flagBlock))
        S_004bd310_ProcessFlagLevelBlock(self, nullptr, flagBlock);
    if (C_0083e9d0_CheckInnerBlockFlagLevel(flagLevels, nullptr, 2, flagBlock))
        S_004bd310_ProcessFlagLevelBlock(self, nullptr, flagBlock);
    if (C_0083e9d0_CheckInnerBlockFlagLevel(flagLevels, nullptr, 1, flagBlock))
        S_004bd310_ProcessFlagLevelBlock(self, nullptr, flagBlock);
    if (C_0083e9d0_CheckInnerBlockFlagLevel(flagLevels, nullptr, 0, flagBlock))
        S_004bd310_ProcessFlagLevelBlock(self, nullptr, flagBlock);

    CSF_AT(uint8_t, g_pCurrentScene, 0x144) = 0;

    S_004f0cd0* slots = CSF_AT(
        S_004f0cd0*, reinterpret_cast<void*>(self->field_0000), 0x40);
    uint32_t* slotBlock = reinterpret_cast<uint32_t*>(
        ClearPendingFlagAndGetInnerBlockAddress(g_pCurrentScene, nullptr));

    if (S_004f0cd0_ResolveSlotIntoOutput(slots, nullptr, 2, slotBlock))
        S_004bd310_ProcessSlotBlock(self, nullptr, slotBlock);
    if (S_004f0cd0_ResolveSlotIntoOutput(slots, nullptr, 1, slotBlock))
        S_004bd310_ProcessSlotBlock(self, nullptr, slotBlock);
    if (S_004f0cd0_ResolveSlotIntoOutput(slots, nullptr, 0, slotBlock))
        S_004bd310_ProcessSlotBlock(self, nullptr, slotBlock);

    CSF_AT(uint8_t, g_pCurrentScene, 0x158) = 0;
    return self->field_000c != 0;
}

HOOK(0x004C4700, S_004bd310_EvaluateCoverOptions, "auto");
