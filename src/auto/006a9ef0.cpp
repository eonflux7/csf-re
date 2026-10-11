#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binds to the original callees by address.
CSF_FUNC(uint32_t*, __fastcall, EmbeddedSubobject_Reset, 0x006A7000, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FinalizeGatheringState, 0x006A9A60, (void* self, void* edx));

// Resets the embedded sub-object at +0x40, then tail-forwards to FinalizeGatheringState(this).
static int __fastcall C_0085fe28_ResetEmbeddedAndFinalize(C_0085fe28* self, void* /*edx*/)
{
    EmbeddedSubobject_Reset(static_cast<uint8_t*>(static_cast<void*>(self)) + 0x40, nullptr);
    return FinalizeGatheringState(self, nullptr);
}

HOOK(0x006A9EF0, C_0085fe28_ResetEmbeddedAndFinalize, "auto");
