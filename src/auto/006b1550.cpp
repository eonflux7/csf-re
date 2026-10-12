#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6D9D20, 0x006D9D20, (void* self, void* edx, int a2, int a3));

// Resolves a lookup value from the object's entry at +0x64, then hands the result
// and both stack arguments to the storing routine, which returns its first-argument result.
static int __fastcall AppMain_SetDisplayParameters(void* self, void* /*edx*/, int a2, int a3)
{
    void* entryOwner = CSF_AT(void*, self, 0x64);

    // The original passes a2 in edx (and pushes a2/a3 for the next call to consume).
    int lookupValue = ResolveEntryLookupValue(entryOwner, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)));

    return sub_6D9D20(reinterpret_cast<void*>(static_cast<intptr_t>(lookupValue)), nullptr, a2, a3);
}

HOOK(0x006B1550, AppMain_SetDisplayParameters, "auto");
