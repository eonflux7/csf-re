#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Timed triple-slot buffer: when the timer at +4 has expired, advance the slot
// index (capped below 3) and re-arm the timer to 1.0. While the timer is still
// running, drain it by a2 and write the 3-float sample a3 into the current slot
// (slots start at float offset 5, 3 floats each).
static void __fastcall TimedTripleBufferUpdate(float* self, void* /*edx*/, float deltaTime, float* sample)
{
    if (self[1] <= g_fZero) {
        uint32_t nextIndex = *reinterpret_cast<uint32_t*>(self) + 1;
        if (nextIndex < 3) {
            *reinterpret_cast<uint32_t*>(self) = nextIndex;
            self[1] = 1.0f;
        }
    }

    if (self[1] > g_fZero) {
        int slotIndex = *reinterpret_cast<int*>(self);
        self[1] = self[1] - deltaTime;
        float* slot = self + 3 * slotIndex + 5;
        slot[0] = sample[0];
        slot[1] = sample[1];
        slot[2] = sample[2];
    }
}

HOOK(0x4AD930, TimedTripleBufferUpdate, "auto");
