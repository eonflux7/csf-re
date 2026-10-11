#include "hook.h"
#include "types/auto_classes.h"

// Returns true when the entry lookup value reported by the object's virtual
// vResolveEntryLookupValue (vtable slot 0xA0) is zero or negative.
static bool __fastcall C_0083e160_IsEntryLookupValueNonPositive(C_0083e160* self, void* /*edx*/)
{
    int lookupValue = self->vResolveEntryLookupValue();
    return lookupValue <= 0;
}

HOOK(0x4E1260, C_0083e160_IsEntryLookupValueNonPositive, "auto");
