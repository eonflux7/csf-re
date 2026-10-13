#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x69AD60, __thiscall with no stack args. Bound as the packet specifies.
CSF_FUNC(unsigned int, __fastcall, NetProfile_PrepareValueChoicesBase, 0x0069AD60, (void* self, void* edx));

// Packet-listed vtable reference. The original performs no store to it here.
[[maybe_unused]] static const void* const kNetProfileVtableRef = CSF_VTBL(0x0069AD60);

// Prepares the base state of the object, then forwards to the object held in field_0078.
// The held object's vtable slot 0 is called with the held object as `this` and `self` as its argument.
static int __fastcall NetProfile_RefreshValueChoices(S_0069ad60* self, void* /*edx*/)
{
    NetProfile_PrepareValueChoicesBase(self, nullptr);

    void* valueChoicesObject = reinterpret_cast<void*>(self->field_0078);
    using SlotZeroFn = int(__fastcall*)(void* self, void* edx, S_0069ad60* arg);
    SlotZeroFn slotZero = reinterpret_cast<SlotZeroFn>(CSF_AT(void**, valueChoicesObject, 0)[0]);
    return slotZero(valueChoicesObject, nullptr, self);
}

HOOK(0x0069AE10, NetProfile_RefreshValueChoices, "auto");
