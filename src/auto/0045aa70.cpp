#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindEntryIndexByVector3, 0x004DC5F0, (uint32_t*, void*, float*));
CSF_FUNC(int, __fastcall, EntryState_ResolveRecord, 0x004DC6E0, (void*, void*, int));
static bool __stdcall EntryMatchesFilter(float* key, char a, char b) {
    void* owner = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1fd8);
    auto* entries = reinterpret_cast<uint32_t*>(static_cast<char*>(owner) + 0x34);
    int index = FindEntryIndexByVector3(entries, nullptr, key);
    if (index == -1) return !a && !b;
    int* state = reinterpret_cast<int*>(EntryState_ResolveRecord(entries, nullptr, index));
    return (a && *state == 1) || (b && *state == 2) || (!a && !b && *state == 0);
}
HOOK(0x0045AA70, EntryMatchesFilter, "auto");
