#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __stdcall, SoundSlot_ResetFlags, 0x0041F6C0, (uint32_t* a1, char* a2));

static int __fastcall C_0086c12c_EvaluateSoundSlotState(C_0086c12c* self, void* /*edx*/, int a2)
{
    // a2 is a state record: +0x00 flag byte, +0x04 owner object pointer,
    // +0x15/+0x16 flag bytes, +0x1C saved dword.
    bool shouldSet = false;
    if (CSF_AT(uint8_t, a2, 0x00) == 0) {
        void* owner = CSF_AT(void*, a2, 0x04);
        uint8_t queryResult =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, a2);
        shouldSet = (queryResult == 0);
    }

    int savedValue = CSF_AT(int, a2, 0x1C);
    CSF_AT(int, a2, 0x1C) = 0;

    CSF_AT(uint8_t, a2, 0x00) = shouldSet ? 1 : 0;

    if (!shouldSet) {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        SoundSlot_ResetFlags(&self->field_0004, (char*)a2);
    }

    CSF_AT(int, a2, 0x1C) = savedValue;

    if (CSF_AT(uint8_t, a2, 0x00) == 0) {
        void* owner = CSF_AT(void*, a2, 0x04);
        uint8_t finishedResult =
            ((uint8_t(__fastcall*)(void*, void*, int))CSF_AT(void**, owner, 0)[31])(
                owner, nullptr, a2);
        if (finishedResult != 0) {
            CSF_AT(uint8_t, a2, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a2, 0x00) = 1;
    return 1;
}

HOOK(0x0075A3F0, C_0086c12c_EvaluateSoundSlotState, "auto");
