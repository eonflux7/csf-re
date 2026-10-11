#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuExtrasControl_SetDescription, 0x004D15F0,
         (void* self, void* edx, int a2));

static int __fastcall MenuExtras_ApplyDescriptionToControl(
    void* self, void* /*edx*/, int description)
{
    uint32_t control = CSF_AT(uint32_t, self, 0x34);
    if (control == 0)
        return 0; // Original leaves eax untouched on this path.
    return MenuExtrasControl_SetDescription(
        reinterpret_cast<void*>(control), nullptr, description);
}

HOOK(0x0048E760, MenuExtras_ApplyDescriptionToControl, "auto");
