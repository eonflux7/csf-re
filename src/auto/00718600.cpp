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

// 0x718600: CGuiControl::m_inputState is returned as a raw pointer-sized value.
// Thiscall with no stack args, so ret 0.
static void* __fastcall CGuiControl_GetInputState(CGuiControl* self, void* /*edx*/) {
    return reinterpret_cast<void*>(self->m_inputState);
}

HOOK(0x718600, CGuiControl_GetInputState, "auto");
