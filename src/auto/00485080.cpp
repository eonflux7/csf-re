#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004846D0: __fastcall (this in ecx, edx unused), no stack args, returns void.
CSF_FUNC(void, __fastcall, CharActionFsm_ResetActionState, 0x004846D0, (void* self, void* edx));

// CCharActionFsm::SetFlag (0x00485080), __thiscall, ret 4.
// If the first pointer field (+0x78) is set, and the requested index is below the
// element count (end - begin) / 4, run the reset helper and store the index at +0x90.
static void __fastcall CCharActionFsm_SetIndexIfInRange(CCharActionFsm* self, void* /*edx*/, uint32_t index)
{
    uint32_t begin = self->field_0078;
    if (begin)
    {
        int32_t elementCount = (int32_t)(self->field_007c - begin) >> 2;
        if (index < (uint32_t)elementCount)
        {
            CharActionFsm_ResetActionState(self, nullptr);
            self->field_0090 = index;
        }
    }
}

HOOK(0x00485080, CCharActionFsm_SetIndexIfInRange, "auto");
