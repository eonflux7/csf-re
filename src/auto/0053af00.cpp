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

// sub_53AC90: __thiscall, one stack argument (ret 4). Bound as __fastcall with
// an explicit edx passthrough because the original call site keeps edx = a2.
CSF_FUNC(int, __fastcall, CMenuOptions_RouteButtonClick, 0x0053AC90, (CMenuOptions* self, void* edx, int a2));

// 0x0053AF00 - Event dispatcher for CMenuOptions.
// Reads the event id pointed to by a2; if it is 13 it delegates to sub_53AC90,
// otherwise it simply returns (id - 13).
static int __fastcall CMenuOptions_HandleMenuEvent(CMenuOptions* self, void* /*edx*/, int* event)
{
    int result = *event - 13;
    if (result == 0)
        return CMenuOptions_RouteButtonClick(self, event, reinterpret_cast<int>(event));
    return result;
}

HOOK(0x0053AF00, CMenuOptions_HandleMenuEvent, "auto");
