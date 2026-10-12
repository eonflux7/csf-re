#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082e090_OnFallThresholdReached, 0x00415E10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_GravityAccel, 0x0082E0EC);

static char __fastcall C_0082e090_UpdateVerticalSpeedWithGravity(C_0082e090* self, void* /*edx*/, float elapsedSeconds)
{
    char result;

    // Field +0x004 points to a state object; its dword at +0x50 is compared with 11.
    void* stateObject = reinterpret_cast<void*>(self->field_0004);
    bool inState11 = (CSF_AT(int, stateObject, 0x50) == 11);

    if (inState11) {
        result = self->field_0078;
        if (!result) {
            void* globals = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
            float threshold = CSF_AT(float, globals, 0x1840);
            if (self->field_0080 > threshold) {
                C_0082e090_OnFallThresholdReached(self, nullptr);
            }
            // Original reloads the flag after the optional callee.
            result = self->field_0078;
        }
    } else {
        result = self->field_0078;
    }

    if (!result) {
        return result;
    }

    self->field_0088 = self->field_0088 - elapsedSeconds * g_GravityAccel;
    return result;
}

HOOK(0x00416280, C_0082e090_UpdateVerticalSpeedWithGravity, "auto");
