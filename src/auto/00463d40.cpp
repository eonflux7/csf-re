#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Scene-wide CHandleTable instance that resolves handles to live objects.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// CHandleTable::Resolve: __thiscall(this, handle), callee pops 4 bytes.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Stack-built request block passed to the target object's slot 17 (36 bytes).
// Layout copied from the original frame: vtable, two -1 keys, a flag byte, a
// 3-int vector, a float that is always -1.0f, and a mode value (7 or 8).
struct MoveOrderRequest {
    void*   vtbl;           // +0x00  request vtable CSF_VTBL(0x0082D448)
    int32_t targetKeyA;     // +0x04  always -1
    int32_t targetKeyB;     // +0x08  always -1
    uint8_t hasVector;      // +0x0C  set to 1 before dispatch
    uint8_t pad_0D[3];
    int32_t vecX;           // +0x10
    int32_t vecY;           // +0x14
    int32_t vecZ;           // +0x18
    float   speedOrNegOne;  // +0x1C  always -1.0f
    int32_t moveMode;       // +0x20  7, or 8 when target flags bit 1 is set
};

static inline uint8_t* AddressToPtr(uint32_t value) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(value));
}

static inline int PtrToInt(const uint8_t* p) {
    return static_cast<int>(reinterpret_cast<uintptr_t>(p));
}

static int __stdcall DispatchMoveOrderByHandle(int /*unused1*/, int /*unused2*/, uint32_t* commandRef) {
    // commandRef[0] -> entity; entity+4 -> handle block; handle block+12 = signed handle index.
    uint8_t* entity = AddressToPtr(commandRef[0]);
    uint8_t* handleBlock = AddressToPtr(CSF_AT(uint32_t, entity, 4));
    int32_t handleIndex = CSF_AT(int32_t, handleBlock, 12);

    int result = PtrToInt(handleBlock + 4);
    if (handleIndex < 0) {
        return result;
    }

    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
    if (result == 0) {
        return result;
    }

    uint8_t* resolved = AddressToPtr(static_cast<uint32_t>(result));
    if (CSF_AT(int32_t, resolved, 0x50) != 26) {
        return result;
    }

    // Only objects of type 26 receive the move order; their +0x34 member is the dispatch target.
    uint8_t* targetObject = AddressToPtr(CSF_AT(uint32_t, resolved, 0x34));

    // The command block pointer is taken from commandRef[1] but not dereferenced yet.
    uint32_t commandBlockAddress = commandRef[1];

    // Target flags are read before the command block is dereferenced.
    uint32_t targetFlags = CSF_AT(uint32_t, targetObject, 0x44);
    int32_t moveMode = 7 + ((targetFlags & 2u) != 0 ? 1 : 0);

    // commandBlock+4 leads to a 3-int position vector at +4 of that pointer.
    uint8_t* commandBlock = AddressToPtr(commandBlockAddress);
    uint8_t* vectorBlock = AddressToPtr(CSF_AT(uint32_t, commandBlock, 4)) + 4;

    MoveOrderRequest request;
    request.vtbl = CSF_VTBL(0x0082D448);
    request.targetKeyA = -1;
    request.targetKeyB = -1;
    request.hasVector = 1;
    request.vecX = CSF_AT(int32_t, vectorBlock, 0);
    request.vecY = CSF_AT(int32_t, vectorBlock, 4);
    request.vecZ = CSF_AT(int32_t, vectorBlock, 8);
    request.speedOrNegOne = -1.0f;
    request.moveMode = moveMode;

    return ((int(__fastcall*)(void*, void*, MoveOrderRequest*))CSF_AT(void**, targetObject, 0)[17])(
        targetObject, nullptr, &request);
}

HOOK(0x00463D40, DispatchMoveOrderByHandle, "auto");
