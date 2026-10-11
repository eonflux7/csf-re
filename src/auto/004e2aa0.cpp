#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Original: std::uint32_t *__thiscall sub_4E2AA0(C_0083e6d8 *this, std::uint32_t *a2), ret 4.
// If a2's word at +4 points back to this object and its word at +0 does not,
// clear this object's field at +0x18. Returns a2 unchanged.
static uint32_t* __fastcall C_0083e6d8_ClearStateIfLinkedFromOtherNode(
    C_0083e6d8* self, void* /*edx*/, uint32_t* a2) {
    uint32_t* result = a2;
    if (reinterpret_cast<C_0083e6d8*>(a2[1]) == self &&
        reinterpret_cast<C_0083e6d8*>(a2[0]) != self) {
        self->field_0018 = 0;
    }
    return result;
}

HOOK(0x4E2AA0, C_0083e6d8_ClearStateIfLinkedFromOtherNode, "auto");
