#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Calls vtable slot 0x2A8 (+680) of the child object at childAddr with two int arguments.
// The child's class is not declared, so the vtable is read through CSF_AT.
static int CallChildStateSlot(uint32_t childAddr, int firstArg, int secondArg) {
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childAddr));
    using ChildStateFn = int(__fastcall*)(void* self, void* edx, int a, int b);
    ChildStateFn slotFn = reinterpret_cast<ChildStateFn>(CSF_AT(void**, child, 0)[0x2A8 / 4]);
    return slotFn(child, nullptr, firstArg, secondArg);
}

// Returns 0 unless all four child pointers (+0x1C, +0x10, +0x14, +0x18) are non-null.
// When all are present, calls slot 0x2A8 on each child with (0, 1), then with (1, 0),
// and returns the result of the last call (on the +0x18 child).
static int __fastcall CMenuMpServerInfo_SetChildStatesIfAllPresent(CMenuMpServerInfo* self, void* /*edx*/) {
    if (self->field_001c == 0 || self->field_0010 == 0 ||
        self->field_0014 == 0 || self->field_0018 == 0) {
        return 0;
    }

    CallChildStateSlot(self->field_001c, 0, 1);
    CallChildStateSlot(self->field_0010, 0, 1);
    CallChildStateSlot(self->field_0014, 0, 1);
    CallChildStateSlot(self->field_0018, 0, 1);

    CallChildStateSlot(self->field_001c, 1, 0);
    CallChildStateSlot(self->field_0010, 1, 0);
    CallChildStateSlot(self->field_0014, 1, 0);
    return CallChildStateSlot(self->field_0018, 1, 0);
}

HOOK(0x00530660, CMenuMpServerInfo_SetChildStatesIfAllPresent, "auto");
