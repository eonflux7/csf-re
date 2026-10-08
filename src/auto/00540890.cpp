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

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());
CSF_FUNC(int, __fastcall, Mp_QueryFlagForGlobal, 0x551650, (void* self, void* edx));

// Updates the score bar's visibility/state panel based on the current game state.
// Returns the result of the last virtual call made on the panel, or 0 if no panel exists.
static int __fastcall CMenuMpScoreBar_SetVisibleByState(CMenuMpScoreBar* self, void* /*edx*/)
{
    int panelPresent = static_cast<int>(self->field_0010);
    if (panelPresent == 0)
        return panelPresent;

    // First check: state object at +0x2C, virtual slot 0x70.
    void* stateObject = reinterpret_cast<void*>(self->field_002c);
    uint8_t stateReady = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
        CSF_AT(void**, stateObject, 0)[0x70 / 4])(stateObject, nullptr);

    if (stateReady != 0) {
        void* panel = reinterpret_cast<void*>(self->field_0010);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, panel, 0)[0xA8 / 4])(panel, nullptr, 1);
    }

    // Second check: global value queried through the matcher.
    int globalValue = GetGlobal8B0688();
    int matched = Mp_QueryFlagForGlobal(reinterpret_cast<void*>(globalValue), nullptr);

    void* panel = reinterpret_cast<void*>(self->field_0010);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, panel, 0)[0xA8 / 4])(panel, nullptr, matched != 0 ? 1 : 0);
}

HOOK(0x00540890, CMenuMpScoreBar_SetVisibleByState, "auto");
