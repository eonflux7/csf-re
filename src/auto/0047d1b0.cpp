#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, TaskState_ResetSlot, 0x0047BCB0, (int a1, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall AiAttackTasks_UpdateTaskState(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* state)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    void* savedField = CSF_AT(void*, state, 0x1C);

    uint8_t newFlag;
    if (CSF_AT(uint8_t, state, 0) != 0) {
        newFlag = 0;
    } else {
        void* obj = CSF_AT(void*, state, 4);
        typedef uint8_t(__fastcall * Fn80)(void*, void*, int, void*);
        uint8_t r = ((Fn80)CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr, 0, state);
        newFlag = (r != 0) ? 0 : 1;
    }
    CSF_AT(uint8_t, state, 0) = newFlag;

    Archive_SerializeBaseFields(self, nullptr, (int)state);

    bool zero = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(void*, state, 0x1C) = nullptr;
    if (zero) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        TaskState_ResetSlot((int)(base + 0x10), state);
    }

    zero = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(void*, state, 0x1C) = nullptr;
    if (zero) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        TaskState_ResetSlot((int)(base + 0x38), state);
    }

    zero = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(void*, state, 0x1C) = nullptr;
    if (zero) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        TaskState_ResetSlot((int)(base + 0x24), state);
    }

    zero = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(void*, state, 0x1C) = savedField;
    if (zero) {
        void* obj = CSF_AT(void*, state, 4);
        typedef int(__fastcall * Fn7C)(void*, void*, void*);
        int result = ((Fn7C)CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, state);
        if ((result & 0xFF) != 0) {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x0047D1B0, AiAttackTasks_UpdateTaskState, "auto");
