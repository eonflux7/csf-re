#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_ApplySkinRegionSelection, 0x006C57A0, (C_0085e7b8** self, void* , C_0086754c* a2));

// Sets or clears bit 0x4 of the flags dword at +0x14 when the requested mode is 4,
// then re-applies the skin region selection if that bit actually changed.
// Returns the requested mode's low byte for other modes, the enable flag when
// mode 4 changes nothing, and the callee's result when the update ran.
static char __fastcall FxParticle_SetMode(C_0086754c* self, void* /*edx*/, int requestedMode, char enableFlag)
{
    if (requestedMode != 4)
        return static_cast<char>(requestedMode);

    // Original compares the raw byte of enableFlag against 0/1 derived from the current bit.
    bool bitCurrentlySet = (CSF_AT(uint32_t, self, 0x14) & 4) == 4;
    if (static_cast<uint8_t>(enableFlag) == (bitCurrentlySet ? 1 : 0))
        return enableFlag;

    uint32_t flags = CSF_AT(uint32_t, self, 0x14);
    if (enableFlag != 0)
        flags |= 4;
    else
        flags &= 0xFFFFFFFBu;
    CSF_AT(uint32_t, self, 0x14) = flags;

    C_0085e7b8** globalValue = reinterpret_cast<C_0085e7b8**>(GetGlobalValue_6C5760());
    return static_cast<char>(C_0085e7b8_ApplySkinRegionSelection(globalValue, nullptr, self));
}

HOOK(0x006CA480, FxParticle_SetMode, "auto");
