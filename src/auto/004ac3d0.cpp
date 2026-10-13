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

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));

// Prepares the static AI state, then resets the action mode of the linked
// object (or of the fallback object stored at absolute address 0x2C when no
// linked object exists) to 0.
static int __stdcall CoverTasks_PrepareStateAndClearActionMode(TAiState* a1)
{
    PrepareStaticAiState(a1);
    void* linkedObjectValue = reinterpret_cast<void*>(GetLinkedObjectValue_4DB5C0(a1, nullptr));
    if (linkedObjectValue)
    {
        C_00834050* actionObject = CSF_AT(C_00834050*, linkedObjectValue, 0x28);
        return C_00834050_SetActionMode(actionObject, nullptr, 0);
    }
    else
    {
        // Null base: the original loads the pointer at absolute address 0x2C (xor eax, eax; [eax+0x2c]).
        void* nullBase = nullptr;
        C_00834050* actionObject = CSF_AT(C_00834050*, nullBase, 0x2C);
        return C_00834050_SetActionMode(actionObject, nullptr, 0);
    }
}

HOOK(0x004AC3D0, CoverTasks_PrepareStateAndClearActionMode, "auto");
