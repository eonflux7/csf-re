#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __fastcall, IsBit5Set, 0x006B8190, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxCollision_IntersectAtomicGeometry, 0x006EC580, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtIntersectionLineSphere, 0x007DA4E0, (float* a1, float* a2, float* a3));

static char __fastcall GfxCollision_IntersectAtomic(
    void* self, void* /*edx*/, int atomicArgument, int queryAddress)
{
    const int atomicAddress = atomicArgument;
    if (!atomicAddress)
        return 0;

    void* attachment = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectAttachment(atomicAddress));

    if (static_cast<std::uint8_t>(GetBit2OfField4(attachment, nullptr)) ||
        static_cast<std::uint8_t>(IsBit5Set(attachment, nullptr)))
        return 0;

    const int sphereAddress = RpAtomicGetWorldBoundingSphere(atomicAddress);
    void* query = reinterpret_cast<void*>(queryAddress);

    if (sphereAddress &&
        !RtIntersectionLineSphere(
            CSF_AT(float*, query, 0x8),
            reinterpret_cast<float*>(sphereAddress),
            reinterpret_cast<float*>(&atomicArgument)))
    {
        CSF_AT(char, query, 0x2) = 0;
        return 0;
    }

    void* geometry = CSF_AT(
        void*, reinterpret_cast<void*>(atomicAddress), 0x18);

    if ((CSF_AT(std::uint8_t, geometry, 0xB) & 1) != 0)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, self, 0)[0x58 / 4])(
                self, nullptr, atomicAddress, queryAddress);
    }
    else
    {
        GfxCollision_IntersectAtomicGeometry(atomicAddress, queryAddress);
    }

    return CSF_AT(char, query, 0x2);
}

HOOK(0x006ECDF0, GfxCollision_IntersectAtomic, "auto");
