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

// Global: packed pointer to the character-select state object (0x8B0694).
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// Callees, bound by address (same signatures across the codebase).
CSF_FUNC(char, __fastcall, ConsumeFlagByteAndClear, 0x007483D0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsStateByteAt5FEqual1, 0x007483A0, (void* self, void*));

// Returns nonzero when the secondary menu state is active: the state object's
// selection index (at +4) is not the sentinel 776, and either the flag byte
// consumed-and-cleared or the state byte at offset 5 equals 1.
static int MenuObject_CheckSecondaryState()
{
    int result = 0;

    uint32_t stateAddr = g_CharacterSelectState8B0694;
    if (stateAddr != 0) {
        uint8_t* state = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(stateAddr));
        uint32_t selectionIndex = CSF_AT(uint32_t, state, 4);
        if (selectionIndex != 0) {
            // Original: lea esi, [eax - 0x308]; 0x308 == 776
            uint32_t offsetIndex = selectionIndex - 0x308;
            void* menuObject = reinterpret_cast<void*>(static_cast<uintptr_t>(offsetIndex));
            if (offsetIndex != 0) {
                if (ConsumeFlagByteAndClear(menuObject, nullptr) != 0 ||
                    IsStateByteAt5FEqual1(menuObject, nullptr) != 0) {
                    return 1;
                }
            }
        }
    }
    return result;
}

HOOK(0x00551BF0, MenuObject_CheckSecondaryState, "auto");
