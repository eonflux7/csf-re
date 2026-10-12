#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads the object linked at +0x04. If that link is set and the flag byte at +0x14
// is set, forwards the call to the linked object's vtable slot 2 (+0x08) without
// returning here. Otherwise returns the low byte of the link value, which is
// zero when the flag is clear and the link is set, or the full link value when
// the link is null.
static hook::Tail __fastcall GetLinkedObjectFlagForward(void* self, const uint32_t* args) {
    (void)args;

    uint32_t linkValue = CSF_AT(uint32_t, self, 0x04);
    uint32_t returnValue = linkValue;

    if (linkValue != 0) {
        if (CSF_AT(uint8_t, self, 0x14) != 0) {
            void* linkedObject = reinterpret_cast<void*>(linkValue);
            return hook::tail_jump(linkedObject, (*reinterpret_cast<void***>(linkedObject))[0x08 / 4]);
        }
        // Flag clear: the original overwrites only the low byte of eax with 0.
        returnValue &= 0xFFFFFF00u;
    }

    return hook::tail_ret(returnValue);
}

HOOK_TAIL(0x004383A0, GetLinkedObjectFlagForward, "auto");
