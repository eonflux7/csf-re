#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00718610 (16 bytes): stores a2 into CGuiControl::m_inputState (+0xE0) and returns it.
static int __fastcall CGuiControl_SetInputState(CGuiControl* self, void* /*edx*/, int state)
{
    self->m_inputState = static_cast<uint32_t>(state);
    return state;
}

HOOK(0x718610, CGuiControl_SetInputState, "auto");
