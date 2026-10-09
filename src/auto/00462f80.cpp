#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetEntityPositionPtr, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, LookupCellIndexByPosition, 0x0075CE00, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetCellRecordByIndex, 0x0075CF00, (void* self, void* edx, int a2));

static char __stdcall UpdateTargetCellFromEntityHandle(void* ownerRecord, int /*unusedArg*/, void** handleRecordRef)
{
    // Zero the owner's target position (state+4..+12).
    C_008342e8* targetState = CSF_AT(C_008342e8*, ownerRecord, 4);
    CSF_AT(float, targetState, 4) = g_ZeroVectorX;
    CSF_AT(float, targetState, 8) = g_ZeroVectorY;
    CSF_AT(float, targetState, 12) = g_ZeroVectorZ;

    // Mark the two ids as -1. The original reloads ownerRecord+4 before each id write.
    CSF_AT(int32_t, CSF_AT(C_008342e8*, ownerRecord, 4), 16) = -1;
    CSF_AT(C_008342e8*, ownerRecord, 4)->field_0014 = -1;

    // Handle table entry: v4 = *(*handleRecordRef + 4); kind = v4[12]; address = v4 + 4.
    void* handleEntry = CSF_AT(void*, *handleRecordRef, 4);
    int32_t handleEntryKind = CSF_AT(int32_t, handleEntry, 12);
    uintptr_t handleAddress = reinterpret_cast<uintptr_t>(handleEntry) + 4;
    if (handleEntryKind < 0)
        return static_cast<char>(handleAddress);

    int resolvedEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddress));
    if (resolvedEntity == 0)
        return 0;
    void* entity = reinterpret_cast<void*>(resolvedEntity);

    // Copy the entity's position into the owner's target position.
    // The destination pointer is read before the position call and reused after it.
    C_008342e8* positionDestination = CSF_AT(C_008342e8*, ownerRecord, 4);
    int32_t* entityPosition = reinterpret_cast<int32_t*>(GetEntityPositionPtr(entity, nullptr));
    CSF_AT(int32_t, positionDestination, 4) = entityPosition[0];
    CSF_AT(int32_t, positionDestination, 8) = entityPosition[1];
    CSF_AT(int32_t, positionDestination, 12) = entityPosition[2];

    // Cell manager is stored at GetValue8B0554() + 8152.
    void* globalBlock = reinterpret_cast<void*>(GetValue8B0554());
    C_0083e0d8* cellManager = CSF_AT(C_0083e0d8*, globalBlock, 8152);

    // Two-dword output buffer for the lookup: [0] = cell index, [1] = cell slot index. Both start at -1.
    int32_t cellLookup[2] = {-1, -1};
    float* entityPositionForLookup = reinterpret_cast<float*>(GetEntityPositionPtr(entity, nullptr));
    char cellFound = LookupCellIndexByPosition(cellManager, nullptr, entityPositionForLookup,
                                               reinterpret_cast<uint32_t*>(cellLookup));
    if (cellFound == 0)
        return 0;

    // Cell record: first dword goes to the owner's id at +16; +0x38 points to a 44-byte record array.
    int32_t cellIndex = cellLookup[0];
    int32_t* cellRecord = reinterpret_cast<int32_t*>(GetCellRecordByIndex(cellManager, nullptr, cellIndex));
    int32_t cellId = cellRecord[0];
    CSF_AT(int32_t, CSF_AT(C_008342e8*, ownerRecord, 4), 16) = cellId;

    int32_t cellSlotIndex = cellLookup[1];
    uint8_t* cellArray = CSF_AT(uint8_t*, cellRecord, 0x38);

    // The owner pointer is reloaded before the selected array value is read.
    C_008342e8* ownerState = CSF_AT(C_008342e8*, ownerRecord, 4);
    int32_t cellArrayValue = *reinterpret_cast<int32_t*>(cellArray + 44 * cellSlotIndex);
    ownerState->field_0014 = cellArrayValue;
    return static_cast<char>(reinterpret_cast<uintptr_t>(ownerState));
}

HOOK(0x00462F80, UpdateTargetCellFromEntityHandle, "auto");
