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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, DispatchWithRecord_51F890, 0x0051F890, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, LookupEntryByKey, 0x007686E0, (void* self, void* edx, int a2));

// Takes the record referenced by the third argument, keeps its entry pointer,
// looks up the entry's key in the manager stored at +0x1F90 of the
// GetValue8B0554() singleton, then dispatches the lookup result through the
// global value and returns that dispatch result.
static int __stdcall ResolveEntryDispatchResult(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // *(a3) -> record; record+4 -> entry
    uint32_t* argRef = reinterpret_cast<uint32_t*>(a3);
    uint32_t* record = reinterpret_cast<uint32_t*>(argRef[0]);
    uint32_t* entry = reinterpret_cast<uint32_t*>(record[1]);

    int manager = GetValue8B0554();
    void* lookupTable = CSF_AT(void*, reinterpret_cast<void*>(manager), 0x1F90);

    // entry+4 -> key, read after the call as the original does
    int key = static_cast<int>(entry[1]);

    int lookupResult = LookupEntryByKey(lookupTable, nullptr, key);

    int32_t globalValue = GetGlobalValue_51F740();

    return DispatchWithRecord_51F890(reinterpret_cast<void*>(globalValue), nullptr, lookupResult);
}

HOOK(0x0045B190, ResolveEntryDispatchResult, "auto");
