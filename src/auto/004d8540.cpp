#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees of the target (bound by address).
CSF_FUNC(int, __fastcall, sub_4AEB90, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, sub_4D7670, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Per-tick update of the autonomous controller: restarts movement for the
// character when its task is ready, updates cover state, and triggers a
// lookup when the cover timer has run past its threshold.
static void __fastcall C_00834fa8_Update(C_00834fa8* self, void* /*edx*/, float a2)
{
    uint32_t charAddr = self->field_0008;
    void* charObj = reinterpret_cast<void*>(charAddr);

    if (sub_4D7670(charObj, nullptr)) {
        int taskResult = sub_4AEB90(CSF_AT(void*, charObj, 0x30c), nullptr);
        if (taskResult != -1 && (taskResult != 0 || CSF_AT(uint8_t, charObj, 0x33c) == 0)) {
            CharTask_StartMovement(charObj, nullptr,
                reinterpret_cast<int>(static_cast<uint8_t*>(charObj) + 0x164));
        }
    }

    AiCoverAttackTasks_UpdateCoverState(static_cast<uint8_t*>(charObj) + 0x330, nullptr, a2);

    // Re-read the character pointer after the call; callees may change state.
    uint8_t* base = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t coverFlag = CSF_AT(uint8_t, base, 0x330);
    if (coverFlag) {
        float timerValue = CSF_AT(float, base, 0x334);
        float threshold = CSF_AT(float, base, 0x338);
        if (timerValue > threshold) {
            CharTask_LookupByOffset(self, nullptr, static_cast<int>(self->field_000c + 100));
        }
    }
}

HOOK(0x004D8540, C_00834fa8_Update, "auto");
