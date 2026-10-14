#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x004DE550, __thiscall with no stack args (ret 0): ecx = this.
CSF_FUNC(void, __fastcall, S_004de550_ReleaseOwnedResource, 0x004DE550, (void* self, void* edx));

// The packet mislabeled this function address as a vtable at the call site.
// The original stores no vtable pointer in this function, so this is a read-only
// address expression with no memory write.
static inline void* S_004de550_ReferencedVtable() {
    // Code-address annotation: function value, not a vtable.
    return reinterpret_cast<void**>(CSF_ADDR_FN(0x004DE550));
}

// Resets the S_004de550 slot: releases the owned resource if the active flag is set,
// zeroes the nine data dwords (+0x04..+0x24), and marks the slot active. Returns 1.
static char __fastcall S_004de550_Reset(S_004de550* self, void* /*edx*/) {
    (void)S_004de550_ReferencedVtable();

    if (self->field_0000) {
        S_004de550_ReleaseOwnedResource(self, nullptr);
        self->field_0000 = 0;
    }
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0000 = 1;
    return 1;
}

HOOK(0x004DE700, S_004de550_Reset, "auto");
