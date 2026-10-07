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

// 0x004FE4B0 (7 bytes) -- getter for CGuiControl::m_pColorAnim.
static int __fastcall CGuiControl_GetColorAnim(CGuiControl* self, void* /*edx*/)
{
    return static_cast<int>(self->m_pColorAnim);
}

HOOK(0x004FE4B0, CGuiControl_GetColorAnim, "auto");
