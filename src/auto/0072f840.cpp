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

// 0x0072F840: returns field_0200 (this + 0x200).
static int __fastcall CGuiChoiceButton_GetField0200(CGuiChoiceButton* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0200);
}

HOOK(0x0072F840, CGuiChoiceButton_GetField0200, "auto");
