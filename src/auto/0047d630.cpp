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

// Callees bound by address (signatures as given in the packet).
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_47CBE0, 0x0047CBE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void* edx, int));

// Prepares the attack task base, runs the helper on the owner link at +0x08,
// then decrements the counter at +0x204 of the sub-object reached through +0x08 -> +0x14.
static int __fastcall TAtacar_PrepareThenDecrementCounter(TAtacar* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    uint32_t linkedObject = CSF_AT(uint32_t, self, 0x08);
    sub_47CBE0(reinterpret_cast<void*>(linkedObject), nullptr);

    uint32_t linkedObjectAgain = CSF_AT(uint32_t, self, 0x08);
    uint32_t innerObject = CSF_AT(uint32_t, reinterpret_cast<void*>(linkedObjectAgain), 0x14);
    TCharTask* counterOwner = reinterpret_cast<TCharTask*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(innerObject), 0x204));

    return C_DecrementCounterAt30(counterOwner, nullptr, reinterpret_cast<int>(self));
}

HOOK(0x0047D630, TAtacar_PrepareThenDecrementCounter, "auto");
