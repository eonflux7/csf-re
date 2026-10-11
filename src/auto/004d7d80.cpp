#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, CanAutonomousTaskRun, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetTaskSlotResult, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Autonomous-AI tick: if the owning character is ready (CanAutonomousTaskRun),
// consult the task slot at +0x30C; a -1 result aborts, a 0 result starts the
// movement task unless the byte flag at +0x33C is set, and any other result
// starts the movement task directly.
static char __fastcall TAutonomo_StartMovementIfReady(int* self, void* /*edx*/, int a2)
{
    (void)a2;
    uint8_t* charObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));

    char result = CanAutonomousTaskRun(charObj, nullptr);
    if (result) {
        int slotResult = GetTaskSlotResult(CSF_AT(void*, charObj, 0x30C), nullptr);
        if (slotResult != -1) {
            if (slotResult != 0) {
                result = static_cast<char>(CharTask_StartMovement(
                    charObj, nullptr, reinterpret_cast<int>(charObj + 0x164)));
            } else {
                uint8_t blockFlag = CSF_AT(uint8_t, charObj, 0x33C);
                if (blockFlag == 0) {
                    result = static_cast<char>(CharTask_StartMovement(
                        charObj, nullptr, reinterpret_cast<int>(charObj + 0x164)));
                } else {
                    result = static_cast<char>(blockFlag);
                }
            }
        }
    }
    return result;
}

HOOK(0x004D7D80, TAutonomo_StartMovementIfReady, "auto");
