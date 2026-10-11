#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (exact signatures from the packet)
CSF_FUNC(char, __fastcall, RefreshWheelsForVehicle, 0x0041AEC0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, ProcessLastChildMatchingId, 0x00408500, (void* self, void* edx, int a2));

// Refreshes wheel state; if the wheel mode is 1 or 2 and the child-processing
// call succeeds, clears the mode. Returns the processing result, or (mode - 2) otherwise.
static char __fastcall C_0082e3b8_UpdateAndResetWheelState(C_0082e3b8* self, void* /*edx*/, int a2)
{
    if (self->field_000c == 0)
        return 0;

    RefreshWheelsForVehicle(self, nullptr, a2);

    uint32_t wheelMode = self->field_008c;
    if (wheelMode == 1 || wheelMode == 2)
    {
        char processed = ProcessLastChildMatchingId(
            reinterpret_cast<void*>(self->field_0030), nullptr, 25);
        if (processed)
            self->field_008c = 0;
        return processed;
    }

    return static_cast<char>(wheelMode - 2);
}

HOOK(0x0041C510, C_0082e3b8_UpdateAndResetWheelState, "auto");
