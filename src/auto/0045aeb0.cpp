#include <cstdint>
#include "globals.h"

#include "hook.h"

// Mission-script lookup: resolves the key stored in the referenced record and
// stores 1 at owner+1 when the manager lookup misses (returns -1), else 0.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, LookupEntryIndex_4DC5F0, 0x004DC5F0, (void* self, void* edx, float* a2));

static int __stdcall ScriptCommands_SetLookupMissFlag(int a1, int a2, int a3) {
    // a3 -> record pointer; record+0 -> holder; holder+4 -> key.
    uint32_t keyHolder = CSF_AT(uint32_t, a3, 0);
    int key = CSF_AT(int, keyHolder, 4);

    // Manager object returned by GetValue8B0554; its +0x1FD8 field is the sub-table.
    uint8_t* managerObj = reinterpret_cast<uint8_t*>(GetValue8B0554());
    uint8_t* subTable = CSF_AT(uint8_t*, managerObj, 0x1FD8);
    void* tableSelf = static_cast<void*>(subTable + 0x34);

    int entryIndex = LookupEntryIndex_4DC5F0(
        tableSelf, nullptr,
        reinterpret_cast<float*>(static_cast<uintptr_t>(static_cast<uint32_t>(key + 4))));

    // Re-read a1 after the calls, as the original does.
    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a1, 4));
    CSF_AT(uint8_t, ownerObj, 1) = (entryIndex == -1) ? 1 : 0;

    return a1;
}

HOOK(0x0045AEB0, ScriptCommands_SetLookupMissFlag, "auto");
