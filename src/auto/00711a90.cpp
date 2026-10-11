#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Clamps a transition progress value to [0, 1], stores it on the object, and
// forwards the call to the linked object's slot 6 (offset 0x18). The clamped
// value is returned in eax when there is no linked object.
static hook::Tail __fastcall C_00863d80_SetTransitionProgress(void* self, const uint32_t* args) {
    C_00863d80* obj = static_cast<C_00863d80*>(self);

    float progress;
    std::memcpy(&progress, &args[0], sizeof(progress));

    // NaN passes through unchanged, as in the original's unordered-compare handling.
    if (progress > g_TransitionOne) {
        progress = g_TransitionOne;
    } else if (progress < g_fZero) {
        progress = g_fZero;
    }

    obj->field_0010 = progress;

    uint32_t linked = obj->field_0008;
    if (linked) {
        void* linkedObj = reinterpret_cast<void*>(linked);
        return hook::tail_jump(linkedObj, (*reinterpret_cast<void***>(linkedObj))[0x18 / 4]);
    }

    uint32_t progressBits;
    std::memcpy(&progressBits, &progress, sizeof(progressBits));
    return hook::tail_ret(progressBits, 4);
}

HOOK_TAIL(0x00711A90, C_00863d80_SetTransitionProgress, "audio");
