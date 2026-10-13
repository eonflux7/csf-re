#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall SceneQueries_IsSelectionEligible(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0xE))
        return CSF_AT(uint8_t, self, 0xC);

    const int globalValue = GetGlobalValue_4FBDE0();
    const int firstElement =
        GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* entry = reinterpret_cast<void*>(
        ResolveEntryLookupValue(reinterpret_cast<void*>(firstElement), nullptr));

    void* projector =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0x14 / 4])(entry, nullptr);

    void* sceneObject = CSF_AT(void*, self, 0);
    void* boundsObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[0x28 / 4])(sceneObject, nullptr);

    if (!boundsObject)
    {
        CSF_AT(uint8_t, self, 0xC) = 0;
        CSF_AT(uint8_t, self, 0xE) = 1;
        return CSF_AT(uint8_t, self, 0xC);
    }

    alignas(8) uint8_t boundsBytes[0x3C];
    alignas(8) uint8_t pointBytes[0xC];
    alignas(8) uint8_t lowerProjectionBytes[0x8];
    alignas(8) uint8_t upperProjectionBytes[0xC];

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, boundsObject, 0)[0x2C / 4])(
            boundsObject, nullptr, boundsBytes);

    CSF_AT(int32_t, pointBytes, 0x8) = CSF_AT(int32_t, boundsBytes, 0x8);
    CSF_AT(float, pointBytes, 0x4) =
        CSF_AT(float, boundsBytes, 0x10) + CSF_AT(float, boundsBytes, 0x4);
    CSF_AT(int32_t, pointBytes, 0) = CSF_AT(int32_t, boundsBytes, 0);

    ((void(__fastcall*)(void*, void*, void*, void*))
        CSF_AT(void**, projector, 0)[0x68 / 4])(
            projector, nullptr, upperProjectionBytes, pointBytes);

    CSF_AT(int32_t, pointBytes, 0x8) = CSF_AT(int32_t, boundsBytes, 0x8);
    CSF_AT(float, pointBytes, 0x4) =
        CSF_AT(float, boundsBytes, 0x4) - CSF_AT(float, boundsBytes, 0x10);
    CSF_AT(int32_t, pointBytes, 0) = CSF_AT(int32_t, boundsBytes, 0);

    ((void(__fastcall*)(void*, void*, void*, void*))
        CSF_AT(void**, projector, 0)[0x68 / 4])(
            projector, nullptr, lowerProjectionBytes, pointBytes);

    void* viewport =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0x2C / 4])(entry, nullptr);

    const int32_t height = static_cast<int32_t>(
        CSF_AT(uint32_t, viewport, 0xC) - CSF_AT(uint32_t, viewport, 0x4));
    const double midpoint = static_cast<double>(height) * g_HalfDurationScale;

    const int eligible =
        midpoint > CSF_AT(float, upperProjectionBytes, 0x4) &&
        midpoint < CSF_AT(float, lowerProjectionBytes, 0x4);

    CSF_AT(uint8_t, self, 0xC) = static_cast<uint8_t>(eligible);
    CSF_AT(uint8_t, self, 0xE) = 1;
    return eligible;
}

HOOK(0x0044F200, SceneQueries_IsSelectionEligible, "auto");
