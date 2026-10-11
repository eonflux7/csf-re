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
CSF_FUNC(int, __cdecl, sub_478880, 0x00478880, (int a1));
CSF_FUNC(int, __fastcall, sub_462A80, 0x00462A80, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_00834198_SetField0198, 0x0049CF30, (C_00834198* self, void*));
CSF_FUNC(int, __fastcall, sub_49CF40, 0x0049CF40, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_4E9540, 0x004E9540, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, sub_6AB8D0, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6AE2F0, 0x006AE2F0, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __fastcall, sub_75CE20, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_75CF00, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_75CF20, 0x0075CF20, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, InitDefaultFields, 0x00461940, (void* self, void*));

static char __stdcall SpawnObjectWithScaledOffset(int /*a1*/, int /*_60*/, std::uint32_t* a3)
{
    // Entity key: a3[0] -> record -> sub-record -> key at +4
    std::uint8_t* firstRecord = reinterpret_cast<std::uint8_t*>(a3[0]);
    std::uint8_t* secondRecord = reinterpret_cast<std::uint8_t*>(CSF_AT(std::uint32_t, firstRecord, 4));
    const int entityKey = CSF_AT(int, secondRecord, 4);

    // First keyed lookup on the game context object at +0x1FC0
    std::uint8_t* gameRoot = reinterpret_cast<std::uint8_t*>(GetValue8B0554());
    void* keyedTable = CSF_AT(void*, gameRoot, 0x1FC0);
    const int keyIndex = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, keyedTable, 0)[3])(keyedTable, nullptr, entityKey);
    if (keyIndex == -1) {
        return static_cast<char>(-1);
    }
    void* keyedRecord = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, keyedTable, 0)[2])(keyedTable, nullptr, keyIndex);
    const int slotIdentifier = sub_478880(CSF_AT(int, keyedRecord, 0x74));
    if (slotIdentifier == -1) {
        return static_cast<char>(-1);
    }

    // Row table at +0x1FD8 on the game context object
    std::uint8_t* gameRootForTables = reinterpret_cast<std::uint8_t*>(GetValue8B0554());
    C_0083e0d8* rowTable = CSF_AT(C_0083e0d8*, gameRootForTables, 0x1FD8);
    std::uint8_t* sourceRecord = reinterpret_cast<std::uint8_t*>(CSF_AT(std::uint32_t, reinterpret_cast<std::uint8_t*>(a3[1]), 4));
    const int tableKey = CSF_AT(int, sourceRecord, 0x10);
    const int elementKey = CSF_AT(int, sourceRecord, 0x14);

    int tableIndex = sub_75CE20(rowTable, nullptr, tableKey);
    if (tableIndex == -1) {
        return static_cast<char>(-1);
    }
    S_00692b80* tableRecord = reinterpret_cast<S_00692b80*>(sub_75CF00(rowTable, nullptr, tableIndex));
    const int elementIndex = sub_462A80(tableRecord, nullptr, elementKey);
    if (elementIndex == -1) {
        return static_cast<char>(-1);
    }

    std::uint8_t* rowData = reinterpret_cast<std::uint8_t*>(
        sub_75CF20(rowTable, nullptr, reinterpret_cast<uint32_t*>(&tableIndex)));

    // Create the spawned object
    C_0085e7b8* sceneManager = reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
    C_00834198* spawnedObject = reinterpret_cast<C_00834198*>(
        sub_4E9540(sceneManager, nullptr, slotIdentifier, -1, 0, 0));

    std::uint32_t initParams[10];
    InitDefaultFields(initParams, nullptr);
    initParams[6] = static_cast<std::uint32_t>(keyIndex);
    initParams[0] = CSF_AT(std::uint32_t, rowData, 4);
    initParams[1] = CSF_AT(std::uint32_t, rowData, 8);
    initParams[2] = CSF_AT(std::uint32_t, rowData, 12);

    void* spawnedSubObject = &spawnedObject->field_000c;
    ((void(__fastcall*)(void*, void*, std::uint32_t*))CSF_AT(void**, spawnedSubObject, 0)[2])(
        spawnedSubObject, nullptr, initParams);
    C_00834198_SetField0198(spawnedObject, nullptr);

    // Build a transform matrix from the row's two floats and read its translation row
    // sized by the original's frame (0x48 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t matrixHolderBytes[0x48];
    S_006ada10& matrixHolder = *reinterpret_cast<S_006ada10*>(matrixHolderBytes);
    matrixHolder.field_0000 = reinterpret_cast<std::uintptr_t>(RwMatrixCreate());
    bool matrixOwned = true;
    sub_6AE2F0(&matrixHolder, nullptr, CSF_AT(float, rowData, 0x10), CSF_AT(float, rowData, 0x14), 0.0f);

    std::uint8_t* matrixBytes = reinterpret_cast<std::uint8_t*>(matrixHolder.field_0000);
    float offsetVector[3];
    offsetVector[0] = CSF_AT(float, matrixBytes, 0x20);
    offsetVector[1] = CSF_AT(float, matrixBytes, 0x24);
    offsetVector[2] = CSF_AT(float, matrixBytes, 0x28);
    sub_6AB8D0(offsetVector, nullptr);

    // Scale the offset by the factor stored in the third argument's record
    std::uint8_t* scaleRecord = reinterpret_cast<std::uint8_t*>(a3[2]);
    std::uint8_t* scaleBlock = reinterpret_cast<std::uint8_t*>(CSF_AT(std::uint32_t, scaleRecord, 4));
    const float scaleFactor = CSF_AT(float, scaleBlock, 4);
    offsetVector[0] = offsetVector[0] * scaleFactor;
    offsetVector[1] = offsetVector[1] * scaleFactor;
    offsetVector[2] = offsetVector[2] * scaleFactor;

    // Result is discarded by the original; the call itself must still happen.
    sub_49CF40(spawnedObject, nullptr, reinterpret_cast<int>(&offsetVector[0]));

    // Original returns only AL: RwMatrixDestroy's low byte when the matrix is owned, else the flag byte (0).
    if (matrixOwned) {
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHolder.field_0000)));
    }
    return 0;
}

HOOK(0x463BB0, SpawnObjectWithScaledOffset, "auto");
