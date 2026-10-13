#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Advances a highlight/fade latency timer. While the timer (+0x44) is below its
// limit (+0x30), it accumulates the frame delta a2. Once the limit is reached,
// it resets the timer to 0 and sets the state field (+0x00) to 3.
static void __fastcall StepObjectiveFadeTimer(S_004276c0* self, void* /*edx*/, float frameDelta) {
    if (self->field_0044 < self->field_0030) {
        self->field_0044 = frameDelta + self->field_0044;
    } else {
        self->field_0044 = 0.0f;
        self->field_0000 = 3;
    }
}

HOOK(0x004276C0, StepObjectiveFadeTimer, "auto");
