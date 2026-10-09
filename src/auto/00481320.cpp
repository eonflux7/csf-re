#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Scans the pointer array [field_0018, field_001c) and returns the index of the first element
// whose GetCachedObjectProvider value equals targetValue, or -1 when none matches.
static int __fastcall CCharWeaponFsm_FindProviderIndex(CCharWeaponFsm* self, void* /*edx*/, int targetValue)
{
    for (uint32_t i = 0; ; ++i)
    {
        // The original reloads the array start on every iteration.
        uint32_t arrayBegin = self->field_0018;
        if (arrayBegin == 0)
            break;

        int byteSpan = static_cast<int>(self->field_001c - arrayBegin);
        uint32_t elementCount = static_cast<uint32_t>(byteSpan >> 2);
        if (i >= elementCount)
            break;

        C_0085e7b8* element = reinterpret_cast<C_0085e7b8**>(arrayBegin)[i];
        void* provider = GetCachedObjectProvider(element, nullptr);
        if (static_cast<int>(reinterpret_cast<uintptr_t>(provider)) == targetValue)
            return static_cast<int>(i);
    }
    return -1;
}

HOOK(0x00481320, CCharWeaponFsm_FindProviderIndex, "auto");
