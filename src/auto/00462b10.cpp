#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindOwnerIndexForKey, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetOwnerEntryByIndex, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GrowPairListStorage, 0x0045BBA0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __stdcall, EntryMatchesFilter, 0x0045AA70, (int a1, char a2, char a3));

// pairList layout: [0] = storage (8-byte pairs), [1] = capacity, [2] = count, [3] = growth step.
static const void* __stdcall AppendMatchingEntryPairs(uint32_t* pairList, int ownerKey, char filterA, char filterB)
{
    void* gameManagerBase = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* ownerTable = CSF_AT(void*, gameManagerBase, 8152);

    int ownerIndex = FindOwnerIndexForKey(ownerTable, nullptr, ownerKey);
    uint32_t result = static_cast<uint32_t>(ownerIndex);
    if (ownerIndex == -1) {
        return reinterpret_cast<const void*>(static_cast<uintptr_t>(result));
    }

    void* ownerEntry = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetOwnerEntryByIndex(ownerTable, nullptr, ownerIndex)));

    // Sub-entries are 44 bytes; the vector spans [+56, +60).
    // Element count = signed (end - begin) / 44, computed as the compiler does for a
    // constant divisor: signed multiply-high by 0x2E8BA2E9, arithmetic shift right by 3,
    // then add one when the shifted value is negative.
    uint32_t elementCount = 0;
    int32_t entryBegin = CSF_AT(int32_t, ownerEntry, 56);
    if (entryBegin != 0) {
        int32_t entryEnd = CSF_AT(int32_t, ownerEntry, 60);
        int32_t entrySpan = entryEnd - entryBegin;
        int64_t product = static_cast<int64_t>(static_cast<int32_t>(0x2E8BA2E9)) * static_cast<int64_t>(entrySpan);
        int32_t highPart = static_cast<int32_t>(product >> 32);
        int32_t shifted = highPart >> 3;
        int32_t signCorrection = static_cast<int32_t>(static_cast<uint32_t>(shifted) >> 31);
        elementCount = static_cast<uint32_t>(shifted + signCorrection);
    }

    result = pairList[1];
    if (elementCount > result) {
        result = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
            GrowPairListStorage(pairList, nullptr, static_cast<int>(elementCount - result))));
    }

    if (elementCount != 0) {
        uint32_t entryIndex = 0;
        int32_t byteOffset = 0;
        do {
            int32_t elementAddress = CSF_AT(int32_t, ownerEntry, 56) + byteOffset + 4;
            bool matched = EntryMatchesFilter(elementAddress, filterA, filterB);
            result = matched;

            if (matched) {
                if (pairList[2] >= pairList[1]) {
                    GrowPairListStorage(pairList, nullptr, static_cast<int>(pairList[3]));
                }
                uint32_t slot = pairList[2];
                uint32_t* pairStorage = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(pairList[0]));
                pairList[2] = slot + 1;
                result = static_cast<uint32_t>(ownerIndex);
                pairStorage[2 * slot] = static_cast<uint32_t>(ownerIndex);
                pairStorage[2 * slot + 1] = entryIndex;
            }

            ++entryIndex;
            byteOffset += 44;
        } while (entryIndex < elementCount);
    }

    return reinterpret_cast<const void*>(static_cast<uintptr_t>(result));
}

HOOK(0x00462B10, AppendMatchingEntryPairs, "auto");
