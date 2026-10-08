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

// Global-state accessors (cdecl, no arguments).
CSF_FUNC(uint8_t*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(uint8_t*, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(void*, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());

// Object refresh helper, thiscall taking one pointer argument (result unused).
CSF_FUNC(int, __fastcall, RefreshGlobalMenuObject, 0x0042DFE0, (void* self, void* edx));

// Readiness check on the menu-controls object, thiscall, returns a byte-sized bool.
CSF_FUNC(char, __fastcall, CanEnterMenuState, 0x0042CDF0, (int** self, void* edx));

static char __fastcall MenuControls_TryActivate(void* self, void* /*edx*/)
{
    uint8_t* state = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, state, 208) != 0)
        return 0;

    uint8_t* value = GetGlobalValue_75FDF0();
    if (CSF_AT(uint8_t, value, 204) == 0)
        return 0;

    RefreshGlobalMenuObject(GetGlobal8afc78(), nullptr);

    char result = CanEnterMenuState(reinterpret_cast<int**>(self), nullptr);
    if (result != 0)
        CSF_AT(uint8_t, self, 0x27C) = 1;

    return result;
}

HOOK(0x00543AF0, MenuControls_TryActivate, "auto");
