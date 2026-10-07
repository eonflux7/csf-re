#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072BCC0 — getter: returns the byte at offset 0x17C (field_017c).
static char __fastcall CGuiPanel_GetField017C(CGuiPanel* self, void* /*edx*/)
{
    return static_cast<char>(self->field_017c);
}

HOOK(0x72BCC0, CGuiPanel_GetField017C, "auto");
