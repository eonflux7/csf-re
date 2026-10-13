#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee: __thiscall with one stack argument (ret 4), bound by address.
CSF_FUNC(int, __fastcall, sub_518780, 0x00518780, (void* self, void* edx, int a2));

// Returns 1 when this->field_0004 equals the value produced by the
// 0x518780 query, 0 otherwise. Original: BOOL __thiscall (ret 0).
static int __fastcall S_00518780_IsFieldEqualToQueryResult(S_00518780* self, void* /*edx*/)
{
    // Reference the struct's vtable as the original does (address-bound).
    [[maybe_unused]] auto* const vtableRef = CSF_VTBL(0x00518780);

    int queryResult = 0;  // out-argument written by the callee
    sub_518780(self, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(&queryResult)));

    // Field is re-read after the call, as in the original.
    return self->field_0004 == static_cast<uint32_t>(queryResult) ? 1 : 0;
}

HOOK(0x00518860, S_00518780_IsFieldEqualToQueryResult, "auto");
