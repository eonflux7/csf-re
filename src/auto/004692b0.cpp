#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Evaluates one node of a condition/action pair. The node's +0 byte holds the
// completion state. Both virtual calls return -1 on failure, which becomes 1 in
// the state byte. The second call (slot 18) only runs when the first left the
// state at 0. The +0x15 byte is saved before the first call and restored before
// the second.
static char __stdcall MissionScript_EvaluatePairedNode(int a1, char* a2)
{
    char result = a2[0];
    char savedFlagByte = a2[21];

    CSF_AT(int, a2, 0x1C) = 0;
    if (a2[0] == 0)
    {
        void* firstTarget = CSF_AT(void*, a2, 4);
        a2[22] = 0;
        int firstReturn = ((int(__fastcall*)(void*, void*, int, char*))CSF_AT(void**, firstTarget, 0)[22])(
            firstTarget, nullptr, a1, a2);
        result = (firstReturn == -1);
        a2[0] = result;
    }

    CSF_AT(int, a2, 0x1C) = 0;
    if (a2[0] == 0)
    {
        void* secondTarget = CSF_AT(void*, a2, 4);
        a2[21] = savedFlagByte;
        a2[22] = 0;
        int secondReturn = ((int(__fastcall*)(void*, void*, int, char*))CSF_AT(void**, secondTarget, 0)[18])(
            secondTarget, nullptr, a1 + 4, a2);
        result = (secondReturn == -1);
        a2[0] = result;
    }

    return result;
}

HOOK(0x004692B0, MissionScript_EvaluatePairedNode, "auto");
