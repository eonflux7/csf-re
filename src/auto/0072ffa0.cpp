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

// Returns m_pageSize of CGuiSlider (offset 0x140).
static int __fastcall CGuiSlider_GetField0140(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<int>(self->m_pageSize);
}

HOOK(0x0072FFA0, CGuiSlider_GetField0140, "auto");
