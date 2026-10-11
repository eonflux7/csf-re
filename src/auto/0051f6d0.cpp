#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __stdcall, GetLookupValueByNameIndex, 0x0051F770, (int nameIndex));
CSF_FUNC(int, __fastcall, ParticleManager_ProcessEntry, 0x00520E20,
         (void* self, void* edx, int arg0, int a3, int a4));

static void __fastcall ParticleManager_ProcessStoredEntry(void* self, void* /*edx*/)
{
    if (CSF_AT(int32_t, self, 0) != 0)
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_51F740());
        int lookupValue = GetLookupValueByNameIndex(20);
        int entry = CSF_AT(int32_t, self, 0);
        int entryData = CSF_AT(int32_t, self, 4);
        ParticleManager_ProcessEntry(manager, nullptr, entry, entryData, lookupValue);
    }
}

HOOK(0x0051F6D0, ParticleManager_ProcessStoredEntry, "auto");
