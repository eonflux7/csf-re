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

// Forwarder that refreshes link state on the owner object; bound by what the caller passes (ecx only).
CSF_FUNC(char, __fastcall, ProcessPendingLinkState, 0x0049DE80, (void* self, void* edx));

CSF_FUNC(char, __fastcall, NotifyDepleted_49D400, 0x0049D400, (void* self, void* edx, void* a3, int a4));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Handles an incoming event record: when its type is 8 and the owner's slot is valid and the
// event id matches the owner's stored id, refresh the owner link state and, if the owner's
// state is 9 (depleted), notify depletion.
static char __fastcall EventMatch_NotifyDepletedOnMatch(void* self, void* /*edx*/, void* eventArg)
{
    char result = static_cast<char>(reinterpret_cast<uintptr_t>(eventArg));

    if (CSF_AT(int, eventArg, 4) == 8 && CSF_AT(int, self, 0x1A0) >= 0)
    {
        if (CSF_AT(int, eventArg, 0xC) == CSF_AT(int, self, 0x19C))
        {
            void* ownerBase = static_cast<uint8_t*>(self) - 0x10;
            result = ProcessPendingLinkState(ownerBase, nullptr);
            if (CSF_AT(int, self, 0x40) == 9)
            {
                result = NotifyDepleted_49D400(ownerBase, nullptr, &g_ZeroVectorX, 0);
            }
        }
    }
    return result;
}

HOOK(0x0049DF00, EventMatch_NotifyDepletedOnMatch, "auto");
