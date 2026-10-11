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

// Bound callees (addresses from the packet).
CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetSession_FinishActivation, 0x00650F90, (void* self, void* edx));

// Original: if the state word at +0x0C equals 1, set state 4 and finish activation;
// otherwise return a derived value computed from that same word.
static int __stdcall NetSession_ActivateWhenStateIsOne(C_0085e760* self)
{
    if (CSF_AT(int, self, 0x0C) != 1)
    {
        // Address-style arithmetic on the state word: (&vtbl[-1].member) + 2.
        int stateWord = CSF_AT(int, self, 0x0C);
        return stateWord - 4 + 2;
    }

    NetSession_SetConnectionState(self, nullptr, 4);
    return (int)NetSession_FinishActivation(self, nullptr);
}

HOOK(0x6A7A40, NetSession_ActivateWhenStateIsOne, "auto");
