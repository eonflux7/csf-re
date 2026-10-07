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

// 0x004FE4B0 (7 bytes) -- getter for CGuiControl::field_00e8.
static int __fastcall CGuiControl_GetField00E8(CGuiControl* self, void* /*edx*/)
{
    return static_cast<int>(self->field_00e8);
}

HOOK(0x004FE4B0, CGuiControl_GetField00E8, "auto");
