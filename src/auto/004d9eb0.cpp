#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall AiAutonomousTask_UpdateState(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* state)
{
    int savedValue = CSF_AT(int, state, 0x1C);

    uint8_t newFlag;
    if (CSF_AT(uint8_t, state, 0x00) != 0) {
        newFlag = 0;
    } else {
        void* owner = CSF_AT(void*, state, 0x04);
        void** vt = CSF_AT(void**, owner, 0);
        int r = ((int(__fastcall*)(void*, void*, int, void*))vt[0x80 / 4])(owner, nullptr, 0, state);
        newFlag = ((uint8_t)r != 0) ? 0 : 1;
    }
    CSF_AT(uint8_t, state, 0x00) = newFlag;

    Archive_SerializeBaseFields(self, nullptr, (int)state);

    bool flagClear = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (flagClear) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        AiAttackTasks_ResetSlot((int)((uint8_t*)self + 0x24), reinterpret_cast<S_00691860*>(state));
    }

    flagClear = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(int, state, 0x1C) = 0;
    if (flagClear) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        AiAttackTasks_ResetSlot((int)((uint8_t*)self + 0x10), reinterpret_cast<S_00691860*>(state));
    }

    flagClear = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(int, state, 0x1C) = savedValue;
    if (flagClear) {
        void* owner = CSF_AT(void*, state, 0x04);
        void** vt = CSF_AT(void**, owner, 0);
        int r = ((int(__fastcall*)(void*, void*, void*))vt[0x7C / 4])(owner, nullptr, state);
        if ((uint8_t)r != 0) {
            CSF_AT(uint8_t, state, 0x00) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x004D9EB0, AiAutonomousTask_UpdateState, "auto");
