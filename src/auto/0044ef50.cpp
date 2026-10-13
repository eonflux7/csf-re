#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Archive helper: runs one optional-member serialization step on a stack-owned
// record. `state` layout: byte 0 = done flag, +4 = driven object, +0x15/+0x16 = cleared
// flags, +0x1C = saved word restored after the step.
// Returns 1 when the step completed, 0 when it was rejected.
static int __fastcall Archive_SerializeOptionalMemberC(void* self, void* /*edx*/, void* state) {
    // Slot 32 (+0x80) on the driven object: returns a byte, only called while not yet done.
    bool stepFinished = CSF_AT(uint8_t, state, 0) == 0 &&
        !((uint8_t(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, CSF_AT(void*, state, 4), 0)[32])(
            CSF_AT(void*, state, 4), nullptr, 0, state);

    int savedWord = CSF_AT(int, state, 0x1c);
    CSF_AT(uint8_t, state, 0) = stepFinished ? 1 : 0;
    CSF_AT(int, state, 0x1c) = 0;

    if (!stepFinished) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        // Slot 14 (+0x38) on the driven object: returns int, result == -1 marks completion.
        int stepResult = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, CSF_AT(void*, state, 4), 0)[14])(
            CSF_AT(void*, state, 4), nullptr, self, state);
        CSF_AT(uint8_t, state, 0) = (stepResult == -1) ? 1 : 0;
    }

    bool stillNotDone = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(int, state, 0x1c) = savedWord;

    // Slot 31 (+0x7C) on the driven object: returns a byte; true means the step is rejected.
    if (stillNotDone &&
        ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, CSF_AT(void*, state, 4), 0)[31])(
            CSF_AT(void*, state, 4), nullptr, state)) {
        CSF_AT(uint8_t, state, 0) = 0;
        return 0;
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x0044EF50, Archive_SerializeOptionalMemberC, "auto");
