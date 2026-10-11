#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Clamps a level value to [0, 1] and stores it on the object. NaN passes through
// unchanged, as in the original's unordered-compare handling. The clamped value is
// then forwarded through the linked object's slot 6 (offset 0x18) with the caller's
// stack arguments untouched; when there is no linked object, the function returns
// the clamped value in eax and pops 4 bytes.
static hook::Tail __fastcall C_00863d80_SetClampedLevel(void* self, const uint32_t* args) {
    C_00863d80* obj = static_cast<C_00863d80*>(self);

    float level;
    std::memcpy(&level, &args[0], sizeof(level));

    // NaN passes through unchanged, as in the original's unordered-compare handling.
    if (level > g_TransitionOne) {
        level = g_TransitionOne;
    } else if (level < g_fZero) {
        level = g_fZero;
    }

    obj->field_0014 = level;

    uint32_t linked = obj->field_000c;
    if (linked) {
        void* linkedObj = reinterpret_cast<void*>(linked);
        return hook::tail_jump(linkedObj, (*reinterpret_cast<void***>(linkedObj))[0x18 / 4]);
    }

    uint32_t levelBits;
    std::memcpy(&levelBits, &level, sizeof(levelBits));
    return hook::tail_ret(levelBits, 4);
}

HOOK_TAIL(0x00711AE0, C_00863d80_SetClampedLevel, "audio");
