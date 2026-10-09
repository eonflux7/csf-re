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
CSF_FUNC(int, __fastcall, sub_4DC5F0, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, sub_4DC6E0, 0x004DC6E0, (void* self, void* edx, int a2));

// Looks up the entry keyed by the value stored in keySource->key and writes
// the resulting pair (or the -1/-1 default) into outRecord->data+4 and +8.
// Returns the address of the written pair.
static int __stdcall LookupPairOutputBySlot(int outRecord, int unusedA2, int keySource) {
    (void)unusedA2;

    void* keyHolder = CSF_AT(void*, keySource, 0);
    uint32_t keyValue = CSF_AT(uint32_t, keyHolder, 4);

    int resultFirst = -1;
    int resultSecond = -1;

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* table = reinterpret_cast<uint8_t*>(CSF_AT(void*, manager, 0x1FD8)) + 0x34;

    int slotIndex = sub_4DC5F0(table, nullptr,
                               reinterpret_cast<float*>(static_cast<uintptr_t>(keyValue + 4)));
    if (slotIndex != -1) {
        void* entry = reinterpret_cast<void*>(sub_4DC6E0(table, nullptr, slotIndex));
        int entryKind = CSF_AT(int32_t, entry, 0);
        if (entryKind == 1 || entryKind == 2) {
            resultFirst = CSF_AT(int32_t, entry, 8);
            resultSecond = CSF_AT(int32_t, entry, 0xC);
        }
    }

    uint8_t* pairDest = reinterpret_cast<uint8_t*>(CSF_AT(void*, outRecord, 4)) + 4;
    CSF_AT(int32_t, pairDest, 4) = resultFirst;
    CSF_AT(int32_t, pairDest, 8) = resultSecond;
    return static_cast<int>(reinterpret_cast<uintptr_t>(pairDest));
}

HOOK(0x00461AF0, LookupPairOutputBySlot, "auto");
