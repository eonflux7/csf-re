#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (by original address).
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, LookupEntryByKey, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, DispatchWithRecord, 0x0051F890, (void* self, void* edx, int a2));

// __stdcall, 3 stack args -> ret 12.
static int __stdcall ResolveEntryDispatchResult(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // owner = *(a3); entry = owner->field_0004 (pointer)
    uint32_t* owner = reinterpret_cast<uint32_t*>(*reinterpret_cast<uint32_t*>(a3));
    uint8_t* entry = reinterpret_cast<uint8_t*>(owner[1]);

    // Original call order: GetValue8B0554 first, then read entry+4 after it.
    int baseHandle = GetValue8B0554();
    void* manager = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(baseHandle) + 0x1F90);
    uint32_t keyValue = *CSF_AT(uint32_t*, entry, 0x4);
    int lookupResult = LookupEntryByKey(manager, nullptr, static_cast<int>(keyValue));

    int globalValue = GetGlobalValue_51F740();
    return DispatchWithRecord(reinterpret_cast<void*>(static_cast<intptr_t>(globalValue)), nullptr, lookupResult);
}

HOOK(0x0045B190, ResolveEntryDispatchResult, "auto");
