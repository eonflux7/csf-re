#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the two choice-number range values (min at +0x1F0, max at +0x1F4) and then
// lets the button refresh itself by invoking the virtual at vtbl+0x2B0 with argument 0.
static int __fastcall CGuiChoiceNumButton_SetRange(
    CGuiChoiceNumButton* self, void* /*edx*/, int a2, int a3)
{
    self->field_01f0 = static_cast<uint32_t>(a2);
    void** vtbl = *reinterpret_cast<void***>(self);
    self->field_01f4 = static_cast<uint32_t>(a3);
    return ((int(__fastcall*)(CGuiChoiceNumButton*, void*, int))vtbl[0x2B0 / 4])(
        self, nullptr, 0);
}

HOOK(0x0072FA30, CGuiChoiceNumButton_SetRange, "auto");
