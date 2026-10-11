#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_424080, 0x00424080, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, sub_408590, 0x00408590, (void* self, void* edx));

// Clears bit 0 of the state word at +0x7C, then sets bit 1 and returns the new state.
// Before that, it calls sub_424080 with the pointer stored at [field_0004 + 0x30]
// and sub_408590 with this->field_0030.
static int __fastcall C_0082e280_ResetWheelStateAndMarkReady(C_0082e280* self, void* /*edx*/)
{
    void* linkedObject = reinterpret_cast<void*>(self->field_0004);
    void* linkedChild = reinterpret_cast<void*>(CSF_AT(uint32_t, linkedObject, 0x30));
    sub_424080(linkedChild, nullptr);

    sub_408590(reinterpret_cast<void*>(self->field_0030), nullptr);

    uint32_t stateWord = self->field_007c & 0xFFFFFFFEu;
    self->field_007c = stateWord;
    self->field_007c = stateWord | 2u;
    return static_cast<int>(stateWord | 2u);
}

HOOK(0x0041B6E0, C_0082e280_ResetWheelStateAndMarkReady, "auto");
