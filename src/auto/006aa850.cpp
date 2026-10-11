#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bound by address; the original passes only `this` (ecx), no stack args.
CSF_FUNC(void*, __fastcall, C_0085fe28_RefreshDdlState, 0x006AA060, (void* self, void* edx));

// Virtual slot 3 (offset 0x0C) returns a byte flag (test al, al).
// Virtual slot 2 (offset 0x08) is a void action taking only `this`.
using PollFlagSlotFn = uint8_t(__fastcall*)(void* self, void* edx);
using ActionSlotFn   = void(__fastcall*)(void* self, void* edx);

static char __fastcall C_0085fe28_RunPollAndMarkInitialized(C_0085fe28* self, void* /*edx*/)
{
    // Original: if (this->vtbl[0xC/4](this)) this->vtbl[0x8/4](this);
    if (reinterpret_cast<PollFlagSlotFn>(CSF_AT(void**, self, 0)[3])(self, nullptr))
    {
        reinterpret_cast<ActionSlotFn>(CSF_AT(void**, self, 0)[2])(self, nullptr);
    }

    C_0085fe28_RefreshDdlState(self, nullptr);

    self->field_0004 = 1;
    return 1;
}

HOOK(0x006AA850, C_0085fe28_RunPollAndMarkInitialized, "auto");
