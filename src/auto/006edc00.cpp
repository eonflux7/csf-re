#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using IntersectionCallback = std::uint32_t* (__cdecl*)(
    int, std::uint32_t*, float, void*);

CSF_FUNC(int, __cdecl, RpAtomicForAllIntersections, 0x0056BDF0, (int atomic, uint32_t* intersection, IntersectionCallback callback, int data));
CSF_FUNC(int, __cdecl, RwV3dTransformVector, 0x00623070, (int a1, int a2, int a3));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(char, __fastcall, GfxCollision_AddIntersection, 0x006ED350, (void* self, void* , float* record, char terminate));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __cdecl GfxCollision_ProcessAtomicIntersection(int atomic, int queryAddress)
{
    void* query = reinterpret_cast<void*>(queryAddress);
    auto* intersection = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(CSF_AT(void*, query, 0x0C)) + 0x1C);

    void* attachment = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectAttachment(atomic));
    if (static_cast<std::uint8_t>(GetBit2OfField4(attachment, nullptr)))
        return atomic;

    alignas(8) std::uint8_t forwardHit[0x2C]{};
    alignas(8) std::uint8_t reverseHit[0x2C]{};
    alignas(8) std::uint8_t reverseIntersection[0x1C]{};

    CSF_AT(std::uint8_t, forwardHit, 0x08) =
        CSF_AT(std::uint8_t, query, 0x0A);
    CSF_AT(std::uint8_t, forwardHit, 0x09) =
        CSF_AT(std::uint8_t, query, 0x0B);
    CSF_AT(std::uint32_t, forwardHit, 0x04) =
        CSF_AT(std::uint32_t, query, 0x04);
    CSF_AT(int, forwardHit, 0x28) = atomic;

    RpAtomicForAllIntersections(
        atomic, intersection,
        reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006EC5F0)),
        reinterpret_cast<int>(forwardHit));

    CSF_AT(std::uint32_t, reverseIntersection, 0x18) = intersection[6];
    CSF_AT(std::uint32_t, reverseIntersection, 0x00) = intersection[3];
    CSF_AT(std::uint32_t, reverseIntersection, 0x04) = intersection[4];
    CSF_AT(std::uint32_t, reverseIntersection, 0x0C) = intersection[0];
    CSF_AT(std::uint32_t, reverseIntersection, 0x10) = intersection[1];
    CSF_AT(std::uint32_t, reverseHit, 0x04) =
        CSF_AT(std::uint32_t, query, 0x04);
    CSF_AT(std::uint8_t, reverseHit, 0x08) =
        CSF_AT(std::uint8_t, query, 0x0A);
    CSF_AT(std::uint32_t, reverseIntersection, 0x08) = intersection[5];
    CSF_AT(std::uint32_t, reverseIntersection, 0x14) = intersection[2];
    CSF_AT(std::uint8_t, reverseHit, 0x09) =
        CSF_AT(std::uint8_t, query, 0x0B);
    CSF_AT(int, reverseHit, 0x28) = atomic;

    RpAtomicForAllIntersections(
        atomic, reinterpret_cast<std::uint32_t*>(reverseIntersection),
        reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006EC5F0)),
        reinterpret_cast<int>(reverseHit));

    if (CSF_AT(std::uint8_t, reverseHit, 0x00))
        CSF_AT(float, reverseHit, 0x0C) =
            g_TransitionOne - CSF_AT(float, reverseHit, 0x0C);

    bool useReverse = false;
    if (!CSF_AT(std::uint8_t, forwardHit, 0x00))
    {
        useReverse = CSF_AT(std::uint8_t, reverseHit, 0x00) != 0;
    }
    else if (CSF_AT(std::uint8_t, reverseHit, 0x00))
    {
        if (CSF_AT(std::uint8_t, forwardHit, 0x10) !=
            CSF_AT(std::uint8_t, reverseHit, 0x10))
        {
            useReverse = CSF_AT(std::uint8_t, reverseHit, 0x10) != 0;
        }
        else
        {
            useReverse = CSF_AT(float, forwardHit, 0x0C) >
                         CSF_AT(float, reverseHit, 0x0C);
        }
    }

    if (useReverse)
        std::memcpy(forwardHit, reverseHit, sizeof(forwardHit));

    if (CSF_AT(std::uint8_t, forwardHit, 0x00))
    {
        // The header and transformed normal belong to one contiguous record.
        alignas(8) std::uint8_t record[0x68]{};
        CSF_AT(std::uint8_t, record, 0x30) = 0xFF;
        CSF_AT(std::uint8_t, record, 0x20) = 0xFF;
        const std::uint32_t frame =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(atomic), 0x04);
        CSF_AT(int, record, 0x2C) = -1;
        CSF_AT(int, record, 0x1C) = -1;
        CSF_AT(int, record, 0x64) = 10;
        CSF_AT(int, record, 0x24) = atomic;
        CSF_AT(std::uint32_t, record, 0x00) =
            CSF_AT(std::uint32_t, forwardHit, 0x0C);

        if (frame)
        {
            std::uint32_t* matrix = RwFrameGetLTM(frame);
            RwV3dTransformVector(
                reinterpret_cast<int>(record + 0x10),
                reinterpret_cast<int>(forwardHit + 0x14),
                reinterpret_cast<int>(matrix));
        }
        else
        {
            CSF_AT(std::uint32_t, record, 0x10) =
                CSF_AT(std::uint32_t, forwardHit, 0x14);
            CSF_AT(std::uint32_t, record, 0x14) =
                CSF_AT(std::uint32_t, forwardHit, 0x18);
            CSF_AT(std::uint32_t, record, 0x18) =
                CSF_AT(std::uint32_t, forwardHit, 0x1C);
        }

        CSF_AT(std::uint32_t, record, 0x1C) =
            CSF_AT(std::uint32_t, forwardHit, 0x20);
        GfxCollision_AddIntersection(
            CSF_AT(void*, query, 0x0C), nullptr,
            reinterpret_cast<float*>(record),
            CSF_AT(char, forwardHit, 0x10));
    }

    return atomic;
}

HOOK(0x006EDC00, GfxCollision_ProcessAtomicIntersection, "auto");
