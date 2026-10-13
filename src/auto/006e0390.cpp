#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_ApplySkinRegionSelection, 0x006C57A0, (C_0085e7b8** self, void* /*edx*/, C_0086754c* a2));
CSF_FUNC(int, __fastcall, C_00861988_LookupSkinRegion, 0x006E0230, (void* self, void* /*edx*/, int a2));

// Looks up a skin region for the argument; when found, applies it to the
// global skin-region holder and returns the lookup result.
static int __fastcall C_00861988_SelectSkinRegion(C_00861988* self, void* /*edx*/, int a2)
{
    int lookupResult = C_00861988_LookupSkinRegion(self, nullptr, a2);
    int result = lookupResult;
    if (lookupResult)
    {
        C_0086754c* region = reinterpret_cast<C_0086754c*>(lookupResult);
        // The original pushes the region before calling GetGlobalValue (cdecl, no args),
        // then passes its return value as `this` to the skin-selection call.
        int* globalHolder = nullptr;
        (void)globalHolder;
        C_0085e7b8_ApplySkinRegionSelection(
            reinterpret_cast<C_0085e7b8**>(GetGlobalValue_6C5760()),
            nullptr,
            region);
    }
    return result;
}

HOOK(0x6E0390, C_00861988_SelectSkinRegion, "auto");
