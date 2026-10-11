#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original is __thiscall with 3 stack args (ret 12). Owning class is not identified yet.
static void __fastcall CharActionFsm_ApplyActionParameters(void* self, void* /*edx*/,
                                                           uint32_t* a2, int a3, float a4)
{
    CSF_GLOBAL(float, g_fZero, 0x82D4CC);
    CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

    CSF_AT(uint32_t, self, 0x3C) = a2[0];
    CSF_AT(uint32_t, self, 0x40) = a2[1];
    CSF_AT(float, self, 0x20) = a4;
    CSF_AT(int, self, 0x28) = a3;

    if (a4 >= g_fZero) {
        if (a4 > g_TransitionOne) {
            CSF_AT(float, self, 0x20) = 1.0f;
        }
    } else {
        CSF_AT(float, self, 0x20) = 0.0f;
    }
}

HOOK(0x4A11A0, CharActionFsm_ApplyActionParameters, "auto");
