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

static CSF_FUNC(int, __fastcall, SkinNode_LookupValue, 0x006C4910, (void* self, void* edx, int a2));

// Looks up a value for this skin node. If nothing is found, follows the chained
// node stored at +0x0C (recursively). With no chain, yields 0.
static int* __fastcall C_008670f0_ResolveChainedValue(C_008670f0* self, void* /*edx*/, int* a2, int a3)
{
    const int originalArg = a3;

    int lookupResult = SkinNode_LookupValue(self, nullptr, a3);
    int* resolved = reinterpret_cast<int*>(lookupResult);

    if (resolved == nullptr) {
        C_008670f0* chained = reinterpret_cast<C_008670f0*>(self->field_000c);
        if (chained != nullptr) {
            *a2 = *C_008670f0_ResolveChainedValue(chained, nullptr, &a3, originalArg);
            return a2;
        }
        a3 = 0;
        resolved = &a3;
    }

    *a2 = *resolved;
    return a2;
}

HOOK(0x006C4960, C_008670f0_ResolveChainedValue, "auto");
