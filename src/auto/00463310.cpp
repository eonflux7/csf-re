#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GridLookupSlotForVector, 0x0075C7C0, (void* self, void* edx, float* a2, int a3, unsigned int* a4));
CSF_FUNC(int, __fastcall, GridGetCellRecord, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GridGetSlotPosition, 0x0075D000, (void* self, void* edx, uint32_t* a2));

namespace {

// Local query block passed by address to GridLookupSlotForVector.
// Stack layout in the original: dword 13 at -0x14, byte 1 at -0x10, dword -1 at -0x0C.
struct SlotLookupQueryBlock
{
    int32_t queryKind;
    uint8_t queryFlag;
    int32_t queryExtra;
};

} // namespace

static uint8_t* AddressToBytes(uint32_t address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

static int __stdcall SyncObjectSlotFromPosition(void* ownerObject, int /*unusedSecondArg*/, void* positionSource)
{
    // Zero the owner's position vector (block = *(ownerObject+4), vector at block+4).
    // The original computes the block address once for these three stores.
    uint8_t* ownerBlock = CSF_AT(uint8_t*, ownerObject, 4);
    CSF_AT(float, ownerBlock, 4) = g_ZeroVectorX;
    CSF_AT(float, ownerBlock, 8) = g_ZeroVectorY;
    CSF_AT(float, ownerBlock, 12) = g_ZeroVectorZ;

    // The original reloads *(ownerObject+4) before each of these two stores.
    CSF_AT(int32_t, CSF_AT(uint8_t*, ownerObject, 4), 16) = -1;
    CSF_AT(int32_t, CSF_AT(uint8_t*, ownerObject, 4), 20) = -1;

    // Manager object: *(GetValue8B0554() + 8152).
    C_0083e0d8* slotManager = CSF_AT(C_0083e0d8*, AddressToBytes(static_cast<uint32_t>(GetValue8B0554())), 8152);

    // Source position: positionSource -> +0 -> +4 -> vector at +4.
    uint8_t* sourceRoot = CSF_AT(uint8_t*, positionSource, 0);
    uint8_t* sourceHolder = CSF_AT(uint8_t*, sourceRoot, 4);
    float sourcePosition[3];
    sourcePosition[0] = CSF_AT(float, sourceHolder, 4);
    sourcePosition[1] = CSF_AT(float, sourceHolder, 8);
    sourcePosition[2] = CSF_AT(float, sourceHolder, 12);

    // Find the slot containing the source position.
    // slotIndices[0] = cell index (a2), slotIndices[1] = entry sub-index (v15).
    SlotLookupQueryBlock lookupQuery{13, 1, -1};
    uint32_t slotIndices[2] = {0xFFFFFFFFu, 0xFFFFFFFFu};
    int lookupFound = GridLookupSlotForVector(
        slotManager,
        nullptr,
        sourcePosition,
        static_cast<int>(reinterpret_cast<uintptr_t>(&lookupQuery)),
        slotIndices);

    int result = lookupFound;
    if (static_cast<uint8_t>(lookupFound) != 0)
    {
        int cellRecordAddress = GridGetCellRecord(slotManager, nullptr, static_cast<int>(slotIndices[0]));
        uint8_t* cellRecord = AddressToBytes(static_cast<uint32_t>(cellRecordAddress));

        // Entry table base at dword index 14 of the cell record; entries are 44 bytes.
        uint32_t cellEntryBase = CSF_AT(uint32_t, cellRecord, 56);
        uint32_t slotEntryAddress = cellEntryBase + slotIndices[1] * 44u;

        // The original reads *(ownerObject+4) here, after GridGetCellRecord and before GridGetSlotPosition.
        uint8_t* ownerBlockForPosition = CSF_AT(uint8_t*, ownerObject, 4);
        int slotPositionAddress = GridGetSlotPosition(slotManager, nullptr, slotIndices);
        uint8_t* slotPosition = AddressToBytes(static_cast<uint32_t>(slotPositionAddress));
        CSF_AT(uint32_t, ownerBlockForPosition, 4) = CSF_AT(uint32_t, slotPosition, 0);
        CSF_AT(uint32_t, ownerBlockForPosition, 8) = CSF_AT(uint32_t, slotPosition, 4);
        CSF_AT(uint32_t, ownerBlockForPosition, 12) = CSF_AT(uint32_t, slotPosition, 8);

        // Cell record's first dword becomes the result and the owner's +16 value.
        int32_t firstCellWord = CSF_AT(int32_t, cellRecord, 0);
        result = firstCellWord;
        CSF_AT(int32_t, CSF_AT(uint8_t*, ownerObject, 4), 16) = firstCellWord;

        // Slot entry's first dword goes to the owner's +20 value.
        uint8_t* slotEntry = AddressToBytes(slotEntryAddress);
        CSF_AT(int32_t, CSF_AT(uint8_t*, ownerObject, 4), 20) = CSF_AT(int32_t, slotEntry, 0);
    }

    return result;
}

HOOK(0x00463310, SyncObjectSlotFromPosition, "auto");
