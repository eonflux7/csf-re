#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindEntryIndexByKey, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GetEntryByIndex, 0x004DC6E0, (void* self, void* edx, int a2));

static int __stdcall ReadEntryValuesIntoResult(int a1, int a2, int a3)
{
    (void)a2;

    // v3 = *(*(a3) + 4)
    uint32_t* keyHolder = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3));
    uint32_t* keyNode = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(*keyHolder));
    uint32_t keyNodeValue = keyNode[1];

    int entryIndex = -1;
    int valueA = -1;   // v4
    int valueB = -1;   // v7

    int singleton = GetValue8B0554();
    uint8_t* singletonPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(singleton));
    uint32_t tableBase = *CSF_AT(uint32_t*, singletonPtr, 0x1FD8) + 0x34;
    void* table = reinterpret_cast<void*>(static_cast<uintptr_t>(tableBase));

    float* keyPtr = reinterpret_cast<float*>(static_cast<uintptr_t>(keyNodeValue + 4));
    entryIndex = FindEntryIndexByKey(table, nullptr, keyPtr);

    if (entryIndex != -1) {
        int* entry = reinterpret_cast<int*>(static_cast<uintptr_t>(GetEntryByIndex(table, nullptr, entryIndex)));
        if (entry[0] == 2) {
            valueA = entry[2];
            valueB = entry[3];
        }
    }

    // result = a1->field_0004 + 4, re-read after the calls
    uint32_t* owner = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1));
    uint32_t result = owner[1] + 4;
    int* outFields = reinterpret_cast<int*>(static_cast<uintptr_t>(result));
    outFields[1] = valueA;
    outFields[2] = valueB;
    return static_cast<int>(result);
}

HOOK(0x461A80, ReadEntryValuesIntoResult, "auto");
