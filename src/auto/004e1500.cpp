#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_74F890, 0x0074F890, (void* self, void* edx));

// If the menu pointer at +0x4C is set and Sub_64B250 reports true for it,
// hand the same object to sub_74F890 (the original tail-jumps there with ecx = object).
static void __fastcall C_0083e330_ProcessMpStatsMenu(C_0083e330* self, void* /*edx*/)
{
    CMenuMpStats* mpStatsMenu = reinterpret_cast<CMenuMpStats*>(self->field_004c);
    if (mpStatsMenu)
    {
        if (Sub_64B250(mpStatsMenu, nullptr))
        {
            sub_74F890(mpStatsMenu, nullptr);
        }
    }
}

HOOK(0x004E1500, C_0083e330_ProcessMpStatsMenu, "auto");
