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

class CMenuMpPause;

CSF_FUNC(char, __cdecl, sub_743820, 0x00743820, ());
CSF_FUNC(void, __fastcall, MenuMpPause_PrepareInitialization, 0x007438A0, (CMenuMpPause* self, void* edx));
CSF_FUNC(int, __cdecl, sub_7438F0, 0x007438F0, ());
CSF_FUNC(uint8_t, __cdecl, Sub_7A8ED0, 0x007A8ED0, ());

CSF_GLOBAL(uint8_t, g_ControlHeldFlag, 0x008B0900);
CSF_GLOBAL(float, g_ControlHoldElapsed, 0x008B08FC);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

static void __cdecl MpRules_UpdateControlState(char a1, float a2)
{
    // The original pushes no argument for Sub_7A8ED0 (`call 0x7a8ed0` directly after `push ebp; mov ebp,esp`),
    // so the callee is declared without parameters and the call pushes nothing.
    if (Sub_7A8ED0())
        return;

    if (a1)
    {
        if (g_ControlHeldFlag)
        {
            g_ControlHeldFlag = 0;
            sub_743820();
            g_ControlHoldElapsed = 0.0f;
        }
    }
    else
    {
        g_ControlHeldFlag = 1;
        if (sub_7438F0())
        {
            g_ControlHoldElapsed = g_ControlHoldElapsed + a2;
            if (g_ControlHoldElapsed > g_ControlHoldThreshold)
            {
                MenuMpPause_PrepareInitialization(nullptr, nullptr);
                g_ControlHoldElapsed = 0.0f;
            }
        }
    }
}

HOOK(0x0055BC30, MpRules_UpdateControlState, "auto");
