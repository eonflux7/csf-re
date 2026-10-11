#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Timer state machine: state at +0x10, flag at +0x08, countdown float at +0x14.
// The original uses ecx as `this` and pops 4 bytes of stack args (ret 4).
static void __fastcall NetCogibleDDL_AdvanceStateMachine(void* self, void* /*edx*/, float deltaTime)
{
    int state = CSF_AT(int, self, 0x10);
    if (state != 0)
    {
        if (state == 1)
        {
            if (CSF_AT(int, self, 0x08) == 0)
            {
                CSF_AT(int, self, 0x10) = 2;
                CSF_AT(int, self, 0x14) = 1084227584; // 5.0f bit pattern
            }
        }
        else if (state == 2)
        {
            float remaining = CSF_AT(float, self, 0x14) - deltaTime;
            CSF_AT(float, self, 0x14) = remaining;
            if (remaining < g_fZero)
            {
                CSF_AT(int, self, 0x10) = 3;
            }
        }
    }
    else if (CSF_AT(int, self, 0x08) != 0)
    {
        CSF_AT(int, self, 0x10) = 1;
    }
}

HOOK(0x740860, NetCogibleDDL_AdvanceStateMachine, "auto");
