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

// sub_55DC00: __thiscall, one stack argument (ret 4). Bound as __fastcall with an
// explicit edx passthrough because the original call site leaves edx = a2 (the event pointer).
CSF_FUNC(int, __fastcall, CMenuPlayerInfo_HandleEvent13, 0x0055DC00, (CMenuPlayerInfo* self, void* edx, int a2));

// 0x0055DD00 - Event dispatcher for CMenuPlayerInfo.
// Reads the event id pointed to by `event`; when the id equals 13 it delegates to
// sub_55DC00, otherwise it simply returns (id - 13). Cloned from CMenuOptions_HandleMenuEvent.
static int __fastcall CMenuPlayerInfo_HandleMenuEvent(CMenuPlayerInfo* self, void* /*edx*/, int* event)
{
    int result = *event - 13;
    if (result == 0)
        return CMenuPlayerInfo_HandleEvent13(self, event, reinterpret_cast<int>(event));
    return result;
}

HOOK(0x0055DD00, CMenuPlayerInfo_HandleMenuEvent, "auto");
