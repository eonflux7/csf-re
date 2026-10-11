#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall Cheats_UpdateAxisBindings(char* self, void* /*edx*/, float* axes)
{
    char* binding = self;
    for (int remaining = 93; remaining != 0; --remaining, binding += 20)
    {
        if (CSF_AT(int, binding, 0x4) != 4)
            continue;

        bool active = false;
        float value = 0.0f;
        switch (CSF_AT(int, binding, 0x8))
        {
        case 0:
            if (axes[4] > g_fZero)
            {
                value = axes[4];
                active = true;
            }
            break;
        case 1:
            if (axes[4] < g_fZero)
            {
                value = axes[4];
                active = true;
            }
            break;
        case 2:
            if (axes[5] > g_fZero)
            {
                value = axes[5];
                active = true;
            }
            break;
        case 3:
            if (axes[5] < g_fZero)
            {
                value = axes[5];
                active = true;
            }
            break;
        case 4:
            if (axes[6] > g_fZero)
            {
                value = axes[6];
                active = true;
            }
            break;
        case 5:
            if (axes[6] < g_fZero)
            {
                value = axes[6];
                active = true;
            }
            break;
        case 6:
            if (axes[7] > g_fZero)
            {
                value = axes[7];
                active = true;
            }
            break;
        case 7:
            if (axes[7] < g_fZero)
            {
                value = axes[7];
                active = true;
            }
            break;
        default:
            break;
        }

        if (active && value != g_fZero)
        {
            if (CSF_AT(int, binding, 0xC) != 2)
            {
                CSF_AT(int, binding, 0xC) = 1;
                CSF_AT(int, binding, 0x14) = 0;
            }
            CSF_AT(float, binding, 0x10) = std::fabs(value);
        }
        else
        {
            const int state = CSF_AT(int, binding, 0xC);
            CSF_AT(int, binding, 0xC) = (state == 1 || state == 2) ? 3 : 0;
        }
    }
}

HOOK(0x00428B90, Cheats_UpdateAxisBindings, "auto");
