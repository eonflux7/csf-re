#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x4382A0: __thiscall method of the child object stored at this+0x2D0.
// Reached via the tail jump with ecx = child object and the char argument forwarded.
CSF_FUNC(int, __fastcall, HudChild_SetEnabled, 0x4382A0, (void* self, void* /*edx*/, char a2));

// 0x42DFF0: forwards the char argument to the child object at +0x2D0 if one exists;
// otherwise does nothing.
static int __fastcall HudWidget_ForwardSetEnabledToChild(void* self, void* /*edx*/, char a2)
{
    uint32_t pChild = CSF_AT(uint32_t, self, 0x2D0);
    if (pChild == 0)
        return 0;                       // original leaves eax untouched on this path
    return HudChild_SetEnabled(reinterpret_cast<void*>(pChild), nullptr, a2);
}

HOOK(0x42DFF0, HudWidget_ForwardSetEnabledToChild, "auto");
