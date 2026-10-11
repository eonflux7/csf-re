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

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

// Resets the task's "active" bit and state when the cached provider entry
// is missing, has the wrong lookup value, or doesn't match the linked object.
static void __fastcall C_0083b118_ResetStateIfProviderInvalid(C_0083b118* self, void* /*edx*/, int /*a2*/)
{
    if (CSF_AT(int, self, 0x40) != 4)
        return;

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(static_cast<intptr_t>(globalValue)), nullptr);
    void* providerRaw = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<intptr_t>(firstElement)), nullptr);
    C_0085e760* provider = reinterpret_cast<C_0085e760*>(providerRaw);

    bool invalid = false;
    if (!provider) {
        invalid = true;
    } else if (ResolveEntryLookupValue(provider, nullptr) != 9) {
        invalid = true;
    } else if (reinterpret_cast<C_0083b958*>(self->field_0014)->field_0020 != CSF_AT(uint32_t, provider, 0xD4)) {
        invalid = true;
    } else if (CSF_AT(uint8_t, provider, 0xCD) != 0) {
        invalid = true;
    }

    if (invalid) {
        self->field_0018 &= ~1u;
        CSF_AT(int, self, 0x40) = 0;
    }
}

HOOK(0x004CF090, C_0083b118_ResetStateIfProviderInvalid, "auto");
