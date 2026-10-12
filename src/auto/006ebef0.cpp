#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsBit5Set, 0x006B8190, (void* self, void*));
CSF_FUNC(int, __fastcall, GetInnerValueAtOffset4, 0x006B81D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxCollision_TestAtomicLineIntersection, 0x006EBA40, (int a1, int a2));
CSF_FUNC(int, __cdecl, RtIntersectionLineSphere, 0x007DA4E0, (float* a1, float* a2, float* a3));

static char __fastcall S_006ebe90_IntersectsObjectLine(
    S_006ebe90* self, void* /*edx*/, int object, int line)
{
    const int atomic = object;
    if (!atomic)
        return 0;

    void* attachment =
        reinterpret_cast<void*>(GfxObjectProps_GetObjectAttachment(atomic));

    if (static_cast<uint8_t>(IsBit5Set(attachment, nullptr)))
        return 0;

    if (!static_cast<uint8_t>(
            ObjectAttachment_HasSoundTrigger(attachment, nullptr)))
    {
        const int inner = GetInnerValueAtOffset4(attachment, nullptr);
        if (!inner || !CSF_AT(int, reinterpret_cast<void*>(inner), 0x14))
            return 0;
    }

    const int sphere = RpAtomicGetWorldBoundingSphere(atomic);
    if (sphere)
    {
        if (!RtIntersectionLineSphere(
                reinterpret_cast<float*>(line),
                reinterpret_cast<float*>(sphere),
                reinterpret_cast<float*>(&object)))
            return 0;
    }

    void* geometry = CSF_AT(void*, reinterpret_cast<void*>(atomic), 0x18);
    if (CSF_AT(uint8_t, geometry, 0x0B) & 1)
    {
        return static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, self, 0)[0x50 / 4])(
                self, nullptr, atomic, line));
    }

    alignas(8) uint8_t queryBytes[0x8];
    CSF_AT(uint8_t, queryBytes, 0) = self->field_0004;
    CSF_AT(char, queryBytes, 1) = 0;
    CSF_AT(int, queryBytes, 4) = line;

    GfxCollision_TestAtomicLineIntersection(
        atomic, reinterpret_cast<int>(queryBytes));
    return CSF_AT(char, queryBytes, 1);
}

HOOK(0x006EBEF0, S_006ebe90_IntersectsObjectLine, "auto");
