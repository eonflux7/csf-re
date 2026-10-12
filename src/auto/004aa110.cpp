#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns non-zero when the cover-action state code at +0x04 is 2 or 3.
static int __fastcall AiCoverAttackTasks_IsCoverActionComplete(void* self, void* /*edx*/)
{
    int actionState = CSF_AT(int, self, 0x4);
    return actionState == 2 || actionState == 3;
}

HOOK(0x4AA110, AiCoverAttackTasks_IsCoverActionComplete, "auto");
