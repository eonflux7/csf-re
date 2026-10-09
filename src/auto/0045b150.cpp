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
CSF_FUNC(int, __fastcall, DispatchWithRecord_51F850, 0x0051F850, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, LookupEntryByKey, 0x007686E0, (void* self, void* edx, int a2));

// Reads the key from the record referenced by the third argument, looks it up
// in the manager stored at +0x1F90 of the GetValue8B0554() singleton, then
// dispatches the lookup result through the global value and the record.
static int __stdcall ResolveSlotLookupResult(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // *(a3) -> record; record+4 -> entry; entry+4 -> key
    uint32_t* argRef = reinterpret_cast<uint32_t*>(a3);
    uint32_t* record = reinterpret_cast<uint32_t*>(argRef[0]);
    uint32_t* entry = reinterpret_cast<uint32_t*>(record[1]);
    int key = static_cast<int>(entry[1]);

    int manager = GetValue8B0554();
    void* lookupTable = CSF_AT(void*, reinterpret_cast<void*>(manager), 0x1F90);

    int lookupResult = LookupEntryByKey(lookupTable, nullptr, key);

    int32_t globalValue = GetGlobalValue_51F740();

    return DispatchWithRecord_51F850(reinterpret_cast<void*>(globalValue), nullptr, lookupResult);
}

HOOK(0x0045B150, ResolveSlotLookupResult, "auto");
