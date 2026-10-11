#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

// Clamps the float argument to [0, 1] (writing the clamped value back to the
// caller's stack slot, as the original does), then tail-jumps to the child
// object's vtable slot 0x38 with the caller's arguments untouched.
static hook::Tail __fastcall C_00863d80_ClampFractionAndForwardToChild(void* self, const uint32_t* args)
{
    C_00863d80* owner = reinterpret_cast<C_00863d80*>(self);

    uint32_t childAddr = owner->field_000c;
    if (childAddr == 0)
        return hook::tail_ret(0, 4);

    float fraction;
    std::memcpy(&fraction, &args[0], sizeof(fraction));

    float clamped = fraction;
    if (fraction <= g_TransitionOne) {
        if (fraction < g_fZero)
            clamped = 0.0f;
    } else {
        clamped = 1.0f;
    }

    if (clamped != fraction) {
        uint32_t bits;
        std::memcpy(&bits, &clamped, sizeof(bits));
        const_cast<uint32_t*>(args)[0] = bits;
    }

    void* child = reinterpret_cast<void*>(childAddr);
    return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x38 / 4]);
}

HOOK_TAIL(0x711B80, C_00863d80_ClampFractionAndForwardToChild, "audio");
