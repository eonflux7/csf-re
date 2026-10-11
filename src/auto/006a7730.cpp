#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0x110 of C_0085fe28's vtable (slot index 68).
using NotifyStateVirtualFn = void(__fastcall*)(void* self, void* edx, int flag);

static void __fastcall C_0085fe28_ResetStateToSeven(C_0085fe28* self, void* /*edx*/, int /*a2*/)
{
    // Only acts when the object is in state 10 and the linked record's
    // field at +0xC is not 1.
    if (self->field_0008 == 10 &&
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0090), 0xC) != 1)
    {
        int pendingFlag = self->field_033c;
        self->field_0008 = 7;
        if (pendingFlag != 0)
        {
            reinterpret_cast<NotifyStateVirtualFn>(CSF_AT(void**, self, 0)[0x110 / 4])(
                self, nullptr, pendingFlag);
        }
    }
}

HOOK(0x6A7730, C_0085fe28_ResetStateToSeven, "auto");
