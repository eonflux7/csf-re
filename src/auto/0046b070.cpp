#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, sub_46A610, 0x0046A610, (void* a1, int a2));

static int __fastcall C_008314c0_EvaluateActionState(C_008314c0* self, void* /*edx*/, int a2)
{
    // a2 is a state record: +0x00 flag byte, +0x04 owner object pointer,
    // +0x15/+0x16 flag bytes, +0x1C saved dword.
    const int stateRecord = a2;

    bool shouldSet = false;
    if (CSF_AT(uint8_t, stateRecord, 0x00) == 0) {
        void* owner = CSF_AT(void*, stateRecord, 0x04);
        uint8_t queryResult =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, stateRecord);
        shouldSet = (queryResult == 0);
    }

    int savedValue = CSF_AT(int, stateRecord, 0x1C);
    CSF_AT(int, stateRecord, 0x1C) = 0;

    CSF_AT(uint8_t, stateRecord, 0x00) = shouldSet ? 1 : 0;

    if (!shouldSet) {
        CSF_AT(uint8_t, stateRecord, 0x15) = 0;
        CSF_AT(uint8_t, stateRecord, 0x16) = 0;
        void* subObject = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 4);
        sub_46A610(subObject, stateRecord);
    }

    CSF_AT(int, stateRecord, 0x1C) = savedValue;

    if (CSF_AT(uint8_t, stateRecord, 0x00) == 0) {
        void* owner = CSF_AT(void*, stateRecord, 0x04);
        uint8_t finishedResult =
            ((uint8_t(__fastcall*)(void*, void*, int))CSF_AT(void**, owner, 0)[31])(
                owner, nullptr, stateRecord);
        if (finishedResult != 0) {
            CSF_AT(uint8_t, stateRecord, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, stateRecord, 0x00) = 1;
    return 1;
}

HOOK(0x0046B070, C_008314c0_EvaluateActionState, "auto");
