// Commandos: Strike Force - HUD module.
// If the object referenced at +0x284 is non-null, the original tail-jumps to 0x4F2F90
// with that object as `this`. Otherwise it returns without calling anything.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x4F2F90 : __thiscall, no stack args (ret 0). Body not yet reimplemented.
CSF_FUNC(int, __fastcall, HudDebugOverlay_Update, 0x4F2F90, (void* self, void* edx));

static int __fastcall HudWidget_ForwardUpdateToSubobject(void* self, void* /*edx*/)
{
    // The original loads the sub-object pointer into ecx and tail-jumps, so the callee
    // receives the sub-object (not the widget) as `this`, and its eax is returned as-is.
    uint32_t pSubobject = CSF_AT(uint32_t, self, 0x284);
    if (pSubobject != 0)
    {
        return HudDebugOverlay_Update(reinterpret_cast<void*>(pSubobject), nullptr);
    }

    // Original leaves eax untouched on this path; the caller-visible value is not
    // reproducible in C++, so 0 is returned.
    return 0;
}

HOOK(0x42DFB0, HudWidget_ForwardUpdateToSubobject, "auto");
