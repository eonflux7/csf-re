#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00720A80 (16 bytes): stores a2 into CGuiStatic::field_015C (+0x15C) and returns it.
static int __fastcall CGuiStatic_SetField015C(CGuiStatic* self, void* /*edx*/, int value)
{
    self->field_015c = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x720A80, CGuiStatic_SetField015C, "auto");
