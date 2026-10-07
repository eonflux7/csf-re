#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00720A70 (16 bytes): stores a2 into CGuiStatic::field_0158 (+0x158) and returns it.
static int __fastcall CGuiStatic_SetField158(CGuiStatic* self, void* /*edx*/, int value)
{
    self->field_0158 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x720A70, CGuiStatic_SetField158, "auto");
