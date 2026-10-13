#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Reads the object linked at +0x08. If that link is set and the flag byte at +0x0C
// is set, forwards the call to the linked object's vtable slot 12 (+0x30) without
// returning here. Otherwise returns the link value with its low byte cleared
// (zero when the link is null).
static hook::Tail __fastcall PrepareUpdateOrForwardLinked(void* self, const uint32_t* args) {
    (void)args;

    C_00863f60* owner = reinterpret_cast<C_00863f60*>(self);

    uint32_t linkedValue = owner->field_0008;
    uint32_t returnValue = linkedValue;

    if (linkedValue != 0) {
        if (owner->field_000c != 0) {
            void* linkedObject = reinterpret_cast<void*>(linkedValue);
            return hook::tail_jump(linkedObject, (*reinterpret_cast<void***>(linkedObject))[0x30 / 4]);
        }
        // Flag clear: the original overwrites only the low byte of eax with 0.
        returnValue &= 0xFFFFFF00u;
    }

    return hook::tail_ret(returnValue);
}

HOOK_TAIL(0x00713E80, PrepareUpdateOrForwardLinked, "auto");
