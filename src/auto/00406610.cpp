#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004054a0_Lookup, 0x004054A0, (void* self, void* edx, int a2));

static int __fastcall S_004054a0_SetFlagAndDispatch(S_004054a0* self, void* /*edx*/, int a2, char a3, char a4)
{
    (void)CSF_VTBL(0x004054A0);

    int lookupResult = S_004054a0_Lookup(self, nullptr, a2);
    if (lookupResult == 0)
        return lookupResult;

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(lookupResult));
    float flagValue = (a3 != 0) ? 1.0f : 0.0f;

    // slot 10 (+0x28) on target: fetch the secondary object
    int secondaryRaw = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, target, 0)[10])(target, nullptr);
    void* secondary = reinterpret_cast<void*>(static_cast<uintptr_t>(secondaryRaw));

    if (a4 != 0)
    {
        // slot 6 (+0x18) on secondary, no arguments
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, secondary, 0)[6])(secondary, nullptr);

        // slot 10 (+0x28) on target again
        int tertiaryRaw = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, target, 0)[10])(target, nullptr);
        void* tertiary = reinterpret_cast<void*>(static_cast<uintptr_t>(tertiaryRaw));

        // slot 1 (+0x04) on tertiary with the flag value
        return reinterpret_cast<int(__fastcall*)(void*, void*, float)>(CSF_AT(void**, tertiary, 0)[1])(tertiary, nullptr, flagValue);
    }

    int storedValue = CSF_AT(int, self, 0x3c);
    // slot 3 (+0x0c) on secondary with (flag value, stored field)
    return reinterpret_cast<int(__fastcall*)(void*, void*, float, int)>(CSF_AT(void**, secondary, 0)[3])(secondary, nullptr, flagValue, storedValue);
}

HOOK(0x00406610, S_004054a0_SetFlagAndDispatch, "auto");
