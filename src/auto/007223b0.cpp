#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x007213C0: __thiscall, ret 0 -> (ecx = this)
CSF_FUNC(int, __fastcall, CGuiEdit_ClearTextAndPositions, 0x7213C0, (void* self, void* edx));
// 0x00722320: __thiscall, ret 4 -> (ecx = this, arg a2)
CSF_FUNC(int*, __fastcall, SynchronizeAttachedObjectText, 0x722320, (void* self, void* edx, int16_t* a2));

// Reads the node pointer stored at +0x134; if it is present, resets the
// container and then forwards the argument to the per-child handler.
static int* __fastcall DetachAndForwardLinkedNode(void* self, void* /*edx*/, int16_t* a2)
{
    int* linked = CSF_AT(int*, self, 0x134);
    if (linked)
    {
        CGuiEdit_ClearTextAndPositions(self, nullptr);
        return SynchronizeAttachedObjectText(self, nullptr, a2);
    }
    return linked;
}

HOOK(0x7223B0, DetachAndForwardLinkedNode, "auto");
