#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the float constant 0.0 (g_fZero) when no effect object is attached,
// otherwise tail-jumps to the attached object's vtable slot 8 (+0x20).
CSF_GLOBAL(float, g_fZero, 0x82D4CC);

static hook::Tail __fastcall C_00863d80_ForwardToEffectOrZero(void* self, const uint32_t* args) {
    (void)args;
    auto* effectHolder = reinterpret_cast<C_00863d80*>(self);

    uint32_t effectObject = effectHolder->field_0008;
    float zeroResult = g_fZero;

    if (effectObject == 0) {
        // Original returns 0.0 in st0 without jumping.
        (void)zeroResult;
        return hook::tail_ret(effectObject);
    }

    // Original: ecx = effect object, then jmp [vtbl + 0x20].
    void* effect = reinterpret_cast<void*>(effectObject);
    return hook::tail_jump(effect, (*reinterpret_cast<void***>(effect))[0x20 / 4]);
}

HOOK_TAIL(0x711BD0, C_00863d80_ForwardToEffectOrZero, "audio");
