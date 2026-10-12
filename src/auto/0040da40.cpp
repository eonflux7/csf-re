#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, ForwardSlot40AndSetFlagsAt18, 0x00424080, (void* self, void*));
CSF_FUNC(uint32_t, __fastcall, ShutdownAllEntries, 0x00408590, (uint32_t* self, void*));

// Forwards the child at [field_0004 + 0x30] to ForwardSlot40AndSetFlagsAt18, shuts down
// the object at field_0030, then clears bit 0 of field_0034, sets bit 1, and returns the new value.
static int __fastcall C_0082dcf0_ShutdownLinkedAndMarkReady(C_0082dcf0* self, void* /*edx*/)
{
    void* linkedObject = reinterpret_cast<void*>(self->field_0004);
    void* linkedChild = reinterpret_cast<void*>(CSF_AT(uint32_t, linkedObject, 0x30));
    ForwardSlot40AndSetFlagsAt18(linkedChild, nullptr);

    ShutdownAllEntries(reinterpret_cast<uint32_t*>(self->field_0030), nullptr);

    uint32_t stateWord = self->field_0034 & 0xFFFFFFFEu;
    self->field_0034 = stateWord;
    self->field_0034 = stateWord | 2u;
    return static_cast<int>(stateWord | 2u);
}

HOOK(0x0040DA40, C_0082dcf0_ShutdownLinkedAndMarkReady, "auto");
