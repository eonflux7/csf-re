#include "hook.h"
#include "types/auto_classes.h"

// Returns true when the attached sub-state pointer at +0x30 is null, or when
// that sub-state's dword at +0x04 is zero. Byte bool return: `mov al, 1` / `xor al, al`.
static bool __fastcall C_00862748_IsPrimaryNullOrIdle(C_00862748* self, void* /*edx*/) {
    S_006dcf90* stateObject = reinterpret_cast<S_006dcf90*>(self->field_0030);
    bool result = true;
    if (stateObject) {
        return stateObject->field_0004 == 0;
    }
    return result;
}

HOOK(0x6F37A0, C_00862748_IsPrimaryNullOrIdle, "auto");
