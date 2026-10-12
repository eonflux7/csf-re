#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue16, 0x004DB5E0, (void* self, void* edx));

// Returns 1 unless a cached object provider exists and its linked object's
// slot +0x1C reports a nonzero state, in which case it returns 0.
static int __fastcall C_0083aa80_IsLinkedObjectIdle(C_0083aa80* self, void* /*edx*/)
{
    void* providerKey = reinterpret_cast<void*>(&self->field_0008);
    int result = 1;

    if (GetCachedObjectProvider(providerKey, nullptr))
    {
        int linkedValue = GetLinkedObjectValue16(providerKey, nullptr);
        void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue));
        int stateFlag = ((int(__fastcall*)(void*, void*))CSF_AT(void**, linkedObject, 0)[0x1C / 4])(linkedObject, nullptr);
        if (stateFlag)
            return 0;
    }
    return result;
}

HOOK(0x004AA4A0, C_0083aa80_IsLinkedObjectIdle, "auto");
