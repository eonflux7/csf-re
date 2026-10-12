#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpWorldForAllAtomicIntersections, 0x56BBD0,
         (int a1, float* a2, int (__cdecl* a3)(float*, int*, int, uint32_t, int), int a4));

// Callback handed to RpWorldForAllAtomicIntersections (address 0x4042A0).
using AtomicIntersectionCallback = int (__cdecl*)(float*, int*, int, uint32_t, int);

static AtomicIntersectionCallback GetAtomicIntersectionCallback() {
    return reinterpret_cast<AtomicIntersectionCallback>(CSF_ADDR_FN(0x4042A0));
}

// Runs the world atomic-intersection query for a bounding box, using a result
// obtained from the owner object's virtual slot 10 (byte offset 0x28).
static int __cdecl WorldCollision_ForAllAtomicIntersections(int world, int* bounds, int userParam, int owner) {
    // 7-dword query block: six bounds values followed by the constant 4.
    int32_t query[7];
    query[0] = bounds[0];
    query[1] = bounds[1];
    query[2] = bounds[2];
    query[3] = bounds[3];
    query[4] = bounds[4];
    query[5] = bounds[5];
    query[6] = 4;

    // Two-dword state block passed to the callback: result, user parameter.
    int32_t state[2] = {0, 0};

    int32_t ownerResult = 0;
    if (owner != 0) {
        void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(owner));
        ownerResult = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerObj, 0)[10])(ownerObj, nullptr);
    }

    state[0] = ownerResult;
    state[1] = userParam;

    return RpWorldForAllAtomicIntersections(
        world,
        reinterpret_cast<float*>(query),
        GetAtomicIntersectionCallback(),
        static_cast<int>(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(state))));
}

HOOK(0x404320, WorldCollision_ForAllAtomicIntersections, "auto");
