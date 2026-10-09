#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, LookupEntryIndex_4DC5F0, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, ResolveEntryStateRecord, 0x004DC6E0, (void* self, void* edx, int a2));

// Looks up an entry in the singleton's table by a key read through a3, then writes the
// entry's two payload values (or -1, -1 when not found / not in state 2) into the
// object at *(a1+4)+4. Returns that target address.
static int __stdcall ScriptCommands_ReadEntryValuesIntoResult(int a1, int a2, int a3)
{
    // v3 = *(*(a3) + 4); the key address is v3 + 4
    void* keyHolder = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a3), 0));
    uint32_t keyBase = CSF_AT(uint32_t, keyHolder, 4);
    uintptr_t keyAddress = static_cast<uintptr_t>(keyBase) + 4;

    int payloadA = -1;   // ebx
    int payloadB = -1;   // ecx

    // singleton = GetValue8B0554(); table = singleton->[0x1FD8] + 0x34
    uintptr_t singletonAddr = static_cast<uintptr_t>(GetValue8B0554());
    void* singleton = reinterpret_cast<void*>(singletonAddr);
    uintptr_t tableAddr = static_cast<uintptr_t>(CSF_AT(uint32_t, singleton, 0x1FD8)) + 0x34;
    void* table = reinterpret_cast<void*>(tableAddr);

    int entryIndex = LookupEntryIndex_4DC5F0(table, nullptr, reinterpret_cast<float*>(keyAddress));
    if (entryIndex != -1) {
        void* record = reinterpret_cast<void*>(
            static_cast<uintptr_t>(ResolveEntryStateRecord(table, nullptr, entryIndex)));
        if (CSF_AT(int, record, 0) == 2) {
            payloadA = CSF_AT(int, record, 8);
            payloadB = CSF_AT(int, record, 12);
        }
    }

    // result = *(a1 + 4) + 4; result[1] = payloadA; result[2] = payloadB
    uintptr_t target = static_cast<uintptr_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4)) + 4;
    CSF_AT(int, reinterpret_cast<void*>(target), 4) = payloadA;
    CSF_AT(int, reinterpret_cast<void*>(target), 8) = payloadB;
    return static_cast<int>(target);
}

HOOK(0x461A80, ScriptCommands_ReadEntryValuesIntoResult, "auto");

