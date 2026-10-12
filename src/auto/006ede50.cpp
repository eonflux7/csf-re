#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(bool, __cdecl, GfxCollision_IsAttachmentExcluded, 0x006EBE50, (void* a1));
CSF_FUNC(int, __cdecl, GfxCollision_ProcessAtomicIntersection, 0x006EDC00, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtIntersectionLineSphere, 0x007DA4E0, (float* a1, float* a2, float* a3));

static void __fastcall C_00862028_TestAtomicIntersection(
    C_00862028* self, void* /*edx*/, int atomic, int query)
{
    const int originalAtomic = atomic;
    if (!originalAtomic)
        return;

    void* attachment = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectAttachment(originalAtomic));
    void* queryData = reinterpret_cast<void*>(query);

    if (!CSF_AT(uint8_t, queryData, 0x0B) &&
        GfxCollision_IsAttachmentExcluded(attachment))
        return;

    const int sphere = RpAtomicGetWorldBoundingSphere(originalAtomic);
    if (sphere)
    {
        auto* intersection = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, queryData, 0x0C));
        if (!RtIntersectionLineSphere(
                reinterpret_cast<float*>(intersection + 0x1C),
                reinterpret_cast<float*>(sphere),
                reinterpret_cast<float*>(&atomic)))
            return;
    }

    void* atomicData = reinterpret_cast<void*>(originalAtomic);
    void* properties = reinterpret_cast<void*>(
        CSF_AT(uint32_t, atomicData, 0x18));
    if (CSF_AT(uint8_t, properties, 0x0B) & 1)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, self, 0)[0x48 / 4])(
                self, nullptr, originalAtomic, query);
    }
    else
    {
        GfxCollision_ProcessAtomicIntersection(originalAtomic, query);
    }
}

HOOK(0x006EDE50, C_00862028_TestAtomicIntersection, "auto");
