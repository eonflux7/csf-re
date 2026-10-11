#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void* edx, int));
CSF_FUNC(int, __fastcall, TCharTask_FinalizeOwner, 0x0047CBE0, (void* self, void* edx));

// Prepares this task, decrements the counter on the owner's chain object (owner->+0x14->+0x204),
// then tail-forwards to the owner's finalizer.
static int __fastcall TDisparar_ReleaseTaskAndForward(TDisparar* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    void* owner = CSF_AT(void*, self, 0x08);
    void* ownerParent = CSF_AT(void*, owner, 0x14);
    void* counterHolder = CSF_AT(void*, ownerParent, 0x204);
    C_DecrementCounterAt30(counterHolder, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    // Tail call: original re-reads this->field_0008 after the decrement.
    return TCharTask_FinalizeOwner(CSF_AT(void*, self, 0x08), nullptr);
}

HOOK(0x0047D960, TDisparar_ReleaseTaskAndForward, "auto");
