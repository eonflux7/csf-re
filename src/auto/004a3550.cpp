#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, UpdateTripleSlotEntry, 0x004A6DD0, (void* self, void* edx, int a2));

// Runs one update pass over a 3-entry table (stride 0x10). The object at a2 holds a
// status byte at +0 and a controller pointer at +4. Returns 1 when the pass finished
// without the controller vetoing it, 0 when the controller's final check fails.
static int __stdcall RunThreeSlotUpdateLoop(S_004a6d30* a1, S_004a6d30* a2)
{
    int stepArg = 3;

    // Initial gate: only consult the controller when the status byte is clear.
    bool keepGoing = false;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* controller = CSF_AT(void*, a2, 4);
        bool vetoed = ((bool(__fastcall*)(void*, void*, int*, S_004a6d30*))
                           CSF_AT(void**, controller, 0)[30])(controller, nullptr, &stepArg, a2);
        keepGoing = !vetoed;
    }
    CSF_AT(uint8_t, a2, 0) = keepGoing ? 1 : 0;

    uint8_t* entry = reinterpret_cast<uint8_t*>(a1);
    unsigned int slotIndex = 0;
    while (true)
    {
        UpdateTripleSlotEntry(entry, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));
        if (CSF_AT(uint8_t, a2, 0) != 0)
            break;

        ++slotIndex;
        entry += 0x10;
        if (slotIndex >= 3)
        {
            // All slots processed and status still clear: final controller check.
            void* controller = CSF_AT(void*, a2, 4);
            if (((bool(__fastcall*)(void*, void*, S_004a6d30*))
                     CSF_AT(void**, controller, 0)[31])(controller, nullptr, a2))
            {
                CSF_AT(uint8_t, a2, 0) = 0;
                return 0;
            }
            break;
        }
    }

    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x004A3550, RunThreeSlotUpdateLoop, "auto");
