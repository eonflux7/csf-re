#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_462B10, 0x00462B10, (void* self, void* edx, uint32_t* a1, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, sub_4DC5F0, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, sub_4DC6E0, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, sub_50DCF0, 0x0050DCF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_75C260, 0x0075C260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_75D000, 0x0075D000, (void* self, void* edx, uint32_t* a2));

static char __fastcall FindGlobalStateEntryAndCopyPair(void* self, void* /*edx*/, int a1, char a2, char a3, int a4)
{
    C_0083e0d8* stateObject = CSF_AT(C_0083e0d8*, reinterpret_cast<void*>(GetValue8B0554()), 8152);

    // a4 points at a record whose +4 field points at a block of two dwords; both are reset to -1.
    uint32_t* outputBlock = reinterpret_cast<uint32_t*>(
        *reinterpret_cast<uintptr_t*>(static_cast<uintptr_t>(a4) + 4) + 4);
    outputBlock[1] = static_cast<uint32_t>(-1);
    outputBlock[2] = static_cast<uint32_t>(-1);

    uint32_t* bucketTable = reinterpret_cast<uint32_t*>(sub_75C260(stateObject, nullptr));
    sub_462B10(self, nullptr, bucketTable, a1, a2, a3);

    unsigned int bucketCount = bucketTable[2];
    if (bucketCount != 0) {
        int globalState = GetGlobalState_50D190();
        unsigned int stateHash = static_cast<unsigned int>(
            sub_50DCF0(reinterpret_cast<void*>(globalState), nullptr));
        unsigned int bucketIndex = stateHash % bucketCount;

        int nodeHandle = sub_75D000(
            stateObject, nullptr,
            reinterpret_cast<uint32_t*>(bucketTable[0] + 8 * bucketIndex));

        void* lookupTable = reinterpret_cast<uint8_t*>(stateObject) + 0x34;
        int entryIndex = sub_4DC5F0(lookupTable, nullptr, reinterpret_cast<float*>(nodeHandle));
        if (entryIndex != -1) {
            uint32_t* foundEntry = reinterpret_cast<uint32_t*>(
                sub_4DC6E0(lookupTable, nullptr, entryIndex));
            uint32_t* outputAgain = reinterpret_cast<uint32_t*>(
                *reinterpret_cast<uintptr_t*>(static_cast<uintptr_t>(a4) + 4) + 4);
            outputAgain[1] = foundEntry[2];
            outputAgain[2] = foundEntry[3];
        }
    }

    stateObject->field_0030 = 0;
    return 0;
}

HOOK(0x00462D00, FindGlobalStateEntryAndCopyPair, "auto");
