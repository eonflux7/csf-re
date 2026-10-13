#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: Sub_64B250 (__fastcall, no stack args)
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Original: sub_74CD70 (__fastcall, 1 stack arg, ret 4)
CSF_FUNC(char, __fastcall, sub_74CD70, 0x0074CD70, (void* self, void* edx, char a2));

// Applies a byte selection (1 or 2) to the menu. If Sub_64B250 reports false, delegates
// to sub_74CD70. Otherwise increments the counter for the matching selection.
static char __fastcall CMenuMpStats_ApplyByteSelection(CMenuMpStats* self, void* /*edx*/, char selection)
{
    if (!Sub_64B250(self, nullptr))
    {
        return sub_74CD70(self, nullptr, selection);
    }

    char result = selection;
    if (selection == 1)
    {
        ++CSF_AT(uint16_t, self, 0x2D6);
    }
    else if (selection == 2)
    {
        ++CSF_AT(uint16_t, self, 0x2DC);
    }
    return result;
}

HOOK(0x00747F60, CMenuMpStats_ApplyByteSelection, "auto");
