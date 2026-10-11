#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

static int __fastcall C_0083b118_UpdateLookupState(C_0083b118* self, void* /*edx*/)
{
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x40)) - 2;
    if (CSF_AT(uint32_t, self, 0x40) == 2)
    {
        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
        void* cachedProvider = GetCachedObjectProvider(
            reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
        result = ResolveEntryLookupValue(cachedProvider, nullptr);
        if (result == 9)
        {
            result = static_cast<int>(self->field_0014);
            if (CSF_AT(uint32_t, cachedProvider, 0xD4) ==
                CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014)), 0x20))
            {
                CSF_AT(uint32_t, self, 0x40) = 4;
                return result;
            }
        }
        self->field_0018 &= ~1u;
        CSF_AT(uint32_t, self, 0x40) = 0;
    }
    return result;
}

HOOK(0x004CF030, C_0083b118_UpdateLookupState, "auto");
