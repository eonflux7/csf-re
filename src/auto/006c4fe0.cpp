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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the Euclidean distance between the 3D points reported by two objects.
// Object a2 yields an intermediate object (slot 4); object a1 yields another
// intermediate object (slot 5); each intermediate yields a pointer to its
// float[3] position (slot 4). The result is the distance between positions B and A.
static double __cdecl GfxSkin_GetInnerPointDistance(int a1, int a2)
{
    if (a1 == 0 || a2 == 0)
        return g_fZero;

    void* objA = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    void* objB = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a1)));

    // v2 = a2->GetInner()  (vtable slot 4, byte offset 0x10)
    void* innerA = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, objA, 0)[4])(objA, nullptr);

    // v3 = a1->GetInner()  (vtable slot 5, byte offset 0x14)
    void* innerB = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, objB, 0)[5])(objB, nullptr);

    // v4 = v2->GetPosition()  (vtable slot 4)
    float* posA = reinterpret_cast<float*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, innerA, 0)[4])(innerA, nullptr));

    // v5 = v3->GetPosition()  (vtable slot 4)
    float* posB = reinterpret_cast<float*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, innerB, 0)[4])(innerB, nullptr));

    float dx = posB[0] - posA[0];
    float dy = posB[1] - posA[1];
    float dz = posB[2] - posA[2];

    return std::sqrt(dz * dz + dy * dy + dx * dx);
}

HOOK(0x006C4FE0, GfxSkin_GetInnerPointDistance, "auto");
