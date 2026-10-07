#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00730110 - __thiscall int CGuiSlider::GetField0148()
CSF_FUNC(int, __fastcall, CGuiSlider_GetField0148, 0x730110, (void* self, void* edx));

// Returns 1 when the embedded sub-object's field_0148 equals 1, otherwise 0.
// The original normalizes the result to a full 32-bit 0/1 value.
static int __fastcall CGuiRadioButton_IsField0148EqualOne(CGuiRadioButton* self, void* /*edx*/)
{
    return CGuiSlider_GetField0148(self, nullptr) == 1;
}

HOOK(0x72FC50, CGuiRadioButton_IsField0148EqualOne, "auto");
