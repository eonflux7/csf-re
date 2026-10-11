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

CSF_FUNC(int, __fastcall, SkinSlot_FindValue, 0x006C4870, (void* self, void* edx, int a2));

// Looks up a value for this slot. If nothing is found, follows the chained
// object stored at +0x0C (recursively). With no chain, yields 0.
static int* __fastcall C_0085e7b8_ResolveChainedValue(C_0085e7b8* self, void* /*edx*/, int* a2, int a3)
{
    const int originalArg = a3;

    int lookupResult = SkinSlot_FindValue(self, nullptr, a3);
    int* resolved = reinterpret_cast<int*>(lookupResult);

    if (resolved == nullptr) {
        C_0085e7b8* chained = reinterpret_cast<C_0085e7b8*>(self->field_000c);
        if (chained != nullptr) {
            *a2 = *C_0085e7b8_ResolveChainedValue(chained, nullptr, &a3, originalArg);
            return a2;
        }
        a3 = 0;
        resolved = &a3;
    }

    *a2 = *resolved;
    return a2;
}

HOOK(0x006C48C0, C_0085e7b8_ResolveChainedValue, "auto");
