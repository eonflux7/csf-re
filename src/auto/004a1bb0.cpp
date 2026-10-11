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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

// Refreshes the description id stored on the selected unit's owner record.
// Returns the value left in eax by the original (global state, 0, the unit pointer, or the parsed description id).
static int __fastcall SelectionController_UpdateSelection(void* self, void* /*edx*/)
{
    int result = GetGlobalState_50D190();
    void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
    if (CSF_AT(uint8_t, globalState, 0xD0) == 0)
        return result;

    uint32_t owner = CSF_AT(uint32_t, self, 8);
    uint32_t entry = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(owner)), 0x18);
    result = static_cast<int>(entry);
    if (result == 0)
        return result;

    result -= 1712;
    if (result == 0)
        return result;

    void* unit = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
    if ((CSF_AT(uint8_t, unit, 0x453) & 1) == 0)
        return result;

    if (CSF_AT(int, unit, 0x668) == -1)
        result = MenuExtras_ParseDescriptionId(CSF_STR(0x834428, "G084"));
    else
        result = MenuExtras_ParseDescriptionId(CSF_STR(0x834430, "G083"));

    // The owner pointer is re-read after the call, as the original does.
    uint32_t ownerAfter = CSF_AT(uint32_t, self, 8);
    CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(ownerAfter)), 0x2AC) = result;
    return result;
}

HOOK(0x004A1BB0, SelectionController_UpdateSelection, "auto");
