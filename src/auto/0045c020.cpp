#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static uint32_t ReadU32(uintptr_t address)
{
    return *reinterpret_cast<const uint32_t*>(address);
}

static int32_t ReadI32(uintptr_t address)
{
    return *reinterpret_cast<const int32_t*>(address);
}

static int __stdcall ComputeHandlePairDistance(int a1, int a2, int* a3)
{
    (void)a2;

    // a3[0] and a3[1] are references to objects whose +4 field points to a record.
    const uintptr_t objectA = static_cast<uint32_t>(a3[0]);
    const uint32_t recordA = ReadU32(objectA + 4);
    const int32_t stateA = ReadI32(recordA + 12);
    const uint32_t handleA = recordA + 4;

    const uintptr_t objectB = static_cast<uint32_t>(a3[1]);
    const uint32_t recordB = ReadU32(objectB + 4);
    const uint32_t handleB = recordB + 4;

    int result = static_cast<int>(handleA);
    if (stateA < 0 || ReadI32(recordB + 12) < 0)
    {
        return result;
    }

    const int resolvedA = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleA));
    const int resolvedB = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleB));
    result = resolvedB;

    if (resolvedA != 0 && resolvedB != 0)
    {
        const uintptr_t selfA = static_cast<uint32_t>(resolvedA);
        const uintptr_t selfB = static_cast<uint32_t>(resolvedB);

        const float* posA = reinterpret_cast<const float*>(
            static_cast<uint32_t>(GetObjectPositionVector(reinterpret_cast<void*>(selfA), nullptr)));
        const float* posB = reinterpret_cast<const float*>(
            static_cast<uint32_t>(GetObjectPositionVector(reinterpret_cast<void*>(selfB), nullptr)));

        const float dx = posB[0] - posA[0];
        const float dy = posB[1] - posA[1];
        const float dz = posB[2] - posA[2];

        const uintptr_t outSlot = static_cast<uint32_t>(a1);
        const uint32_t outRecord = ReadU32(outSlot + 4);
        result = static_cast<int>(outRecord);

        *reinterpret_cast<float*>(static_cast<uintptr_t>(outRecord) + 4) =
            std::sqrt(dz * dz + dx * dx + dy * dy);
    }

    return result;
}

HOOK(0x45C020, ComputeHandlePairDistance, "auto");
