#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (exact signatures from the packet)
CSF_FUNC(char, __fastcall, UpdateTimedState, 0x0040D570, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, ProcessLastChildMatchingId, 0x00408500, (void* self, void* , int targetId));

// Runs the timed state update when the enabled flag is set. If the pending mode is
// 1 or 2 and the child-processing call succeeds, clears the mode. Returns the
// processing result, or (mode - 2) when the mode is neither 1 nor 2.
static char __fastcall C_0082dcf0_UpdateAndResetPendingState(C_0082dcf0* self, void* /*edx*/, float a2)
{
    if (self->field_000c == 0)
        return 0;

    UpdateTimedState(self, nullptr, a2);

    if (self->field_0050 == 1 || self->field_0050 == 2)
    {
        char processed = ProcessLastChildMatchingId(
            reinterpret_cast<void*>(self->field_0030), nullptr, 1);
        if (processed)
            self->field_0050 = 0;
        return processed;
    }

    return static_cast<char>(self->field_0050 - 2);
}

HOOK(0x0040DA00, C_0082dcf0_UpdateAndResetPendingState, "auto");
