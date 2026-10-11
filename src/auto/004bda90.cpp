#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, ResetPendingFlagAndGetInnerBlockAddress, 0x0044FFB0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CheckInnerBlockFlagLevel, 0x004E72C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(bool, __fastcall, AiCore_ApplyInnerBlockAction, 0x004BD8F0, (void* self, void* edx, uint32_t* a2, float* a3));

// Checks the inner block flag levels 3, 2, 1, 0 in order (first hit wins), applies the
// inner block action when any matches, clears the scene's pending byte at +0x144, and
// returns whether the request's target id at +8 is valid (!= -1).
static int __fastcall S_004ad990_ProcessInnerBlockAndCheckValid(S_004ad990* self, void* /*edx*/, int a2)
{
    C_0083e9d0* blockOwner =
        reinterpret_cast<C_0083e9d0*>(reinterpret_cast<S_004ad990*>(self->field_0000)->field_003c);
    int innerBlockAddress = ResetPendingFlagAndGetInnerBlockAddress(g_pCurrentScene, nullptr);

    if (CheckInnerBlockFlagLevel(blockOwner, nullptr, 3, innerBlockAddress)
        || CheckInnerBlockFlagLevel(blockOwner, nullptr, 2, innerBlockAddress)
        || CheckInnerBlockFlagLevel(blockOwner, nullptr, 1, innerBlockAddress)
        || CheckInnerBlockFlagLevel(blockOwner, nullptr, 0, innerBlockAddress))
    {
        AiCore_ApplyInnerBlockAction(self, nullptr,
                                     reinterpret_cast<uint32_t*>(innerBlockAddress),
                                     reinterpret_cast<float*>(a2));
    }

    CSF_AT(uint8_t, g_pCurrentScene, 0x144) = 0;
    return CSF_AT(int, reinterpret_cast<void*>(a2), 0x08) != -1;
}

HOOK(0x004BDA90, S_004ad990_ProcessInnerBlockAndCheckValid, "auto");
