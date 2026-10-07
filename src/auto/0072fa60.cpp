#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the two integer parameters into the button's second value pair
// (+0x1F8, +0x1FC) and then lets the button refresh itself by invoking
// the virtual at vtable +0x2B0 with argument 0, returning its result.
static int __fastcall CGuiChoiceNumButton_SetValueRange(
    CGuiChoiceNumButton* self, void* /*edx*/, int a2, int a3)
{
    self->field_01f8 = static_cast<uint32_t>(a2);
    void** vtbl = *reinterpret_cast<void***>(self);
    self->field_01fc = static_cast<uint32_t>(a3);
    return ((int(__fastcall*)(CGuiChoiceNumButton*, void*, int))vtbl[0x2B0 / 4])(
        self, nullptr, 0);
}

HOOK(0x0072FA60, CGuiChoiceNumButton_SetValueRange, "auto");
