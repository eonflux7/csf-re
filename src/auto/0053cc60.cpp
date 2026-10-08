#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Queries the control object stored at +0x2C through its virtual slot 12
// (offset 0x30) and stores the returned low byte into the flag at +0x3C,
// returning the full 32-bit result to the caller.
static int __fastcall CMenuSoundOptions_UpdateToggleFlagFromControl(CMenuSoundOptions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_002c);

    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[12])(control, nullptr);

    self->field_003c = static_cast<uint8_t>(result);
    return result;
}

HOOK(0x0053CC60, CMenuSoundOptions_UpdateToggleFlagFromControl, "auto");
