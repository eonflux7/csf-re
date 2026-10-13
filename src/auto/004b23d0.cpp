#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, ClearPendingFlagAndGetInnerBlockAddress, 0x0044FFD0, (void* self, void*));
CSF_FUNC(bool, __fastcall, S_004f0cd0_ResolveSlotIntoOutput, 0x004F1530, (S_004f0cd0* self, void*, int a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, S_004ad990_WriteCombatDestination, 0x004B22D0, (void* self, void* edx, uint32_t* a2, int a3));

static int __fastcall S_004ad990_TryBuildCombatDestination(S_004ad990* self, void* /*edx*/, int a2)
{
    S_004f0cd0* slotOwner = reinterpret_cast<S_004f0cd0*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0000), 0x40));
    uint32_t* innerBlockAddress = reinterpret_cast<uint32_t*>(
        ClearPendingFlagAndGetInnerBlockAddress(g_pCurrentScene, nullptr));

    if (S_004f0cd0_ResolveSlotIntoOutput(slotOwner, nullptr, 2, innerBlockAddress)
        || S_004f0cd0_ResolveSlotIntoOutput(slotOwner, nullptr, 1, innerBlockAddress)
        || S_004f0cd0_ResolveSlotIntoOutput(slotOwner, nullptr, 0, innerBlockAddress))
    {
        S_004ad990_WriteCombatDestination(self, nullptr, innerBlockAddress, a2);
    }

    CSF_AT(uint8_t, g_pCurrentScene, 0x158) = 0;

    void* outPtr = reinterpret_cast<void*>(a2);
    return CSF_AT(int, outPtr, 4) != -1;
}

HOOK(0x004B23D0, S_004ad990_TryBuildCombatDestination, "auto");
