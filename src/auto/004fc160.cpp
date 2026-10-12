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

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

// Returns the index of the first entry whose referenced object equals `wantedObject`
// and whose resolved lookup value reports true from virtual slot 18 (+0x48).
// Returns -1 when no entry matches.
static int __fastcall CommandoAnims_FindEntryIndex(uint32_t* self, void* /*edx*/, int wantedObject)
{
    int result = -1;
    uint32_t entryIndex = 0;
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x1C);
    if (entryCount == 0)
        return result;

    for (;;)
    {
        C_0085e760* entry = reinterpret_cast<C_0085e760*>(
            CSF_AT(uint32_t*, self, 0x14)[entryIndex]);

        if (reinterpret_cast<intptr_t>(GetReferencedObject(entry, nullptr)) == wantedObject)
        {
            if (ResolveEntryLookupValue(entry, nullptr))
            {
                int resolvedValue = ResolveEntryLookupValue(entry, nullptr);
                void* resolvedObject = reinterpret_cast<void*>(resolvedValue);
                bool accepted = ((bool(__fastcall*)(void*, void*))
                                    CSF_AT(void**, resolvedObject, 0)[18])(resolvedObject, nullptr);
                if (accepted)
                    return static_cast<int>(entryIndex);
            }
        }

        ++entryIndex;
        if (entryIndex >= entryCount)
            return -1;
    }
}

HOOK(0x004FC160, CommandoAnims_FindEntryIndex, "auto");
