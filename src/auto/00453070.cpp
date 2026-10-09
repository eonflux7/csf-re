#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4511A0, 0x004511A0, (void* self, void* edx, void* a1, float* a2, int* a3, uint32_t* a4, float a5));
CSF_FUNC(int, __fastcall, sub_451250, 0x00451250, (void* self, void* edx, uint32_t* a2, int a3, float a4, int a5));

// Applies the display-flag sub-object update (flags 0x20 / 0x10 select the target
// field), then dispatches to the common handler unless flag 0x2000 is set, in
// which case the current flag word is returned instead.
static int __fastcall CGuiControl_ProcessDisplayFlags(CGuiControl* self, void* /*edx*/,
                                                      void* a2, float* a3, int* a4,
                                                      int a5, float a6)
{
    uint32_t* targetField;
    if ((*a4 & 0x20) != 0) {
        targetField = &self->field_0010;
    } else {
        if ((*a4 & 0x10) == 0)
            goto DispatchTail;
        targetField = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x20);
    }
    sub_4511A0(self, nullptr, a2, a3, a4, targetField, a6);

DispatchTail:
    int result = *a4;
    if ((*a4 & 0x2000) == 0)
        return sub_451250(self, nullptr, reinterpret_cast<uint32_t*>(a2),
                          reinterpret_cast<int>(a3), a6, a5);
    return result;
}

HOOK(0x453070, CGuiControl_ProcessDisplayFlags, "auto");
