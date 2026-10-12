#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GfxObject_GetLodDistanceScale, 0x006BF200, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008656d0_IsMaskSubset, 0x006C5780, (C_008656d0* self, void* , int mask));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall S_006b30b0_ClassifyDistanceLod(
    S_006b30b0* self, void* /*edx*/)
{
    void* view = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* transform = reinterpret_cast<std::uint8_t*>(self) + 0x30;
    const float* position =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    const float dx = CSF_AT(float, view, 0x124) - position[0];
    const float dy = CSF_AT(float, view, 0x128) - position[1];
    const float dz = CSF_AT(float, view, 0x12C) - position[2];

    void* distanceSettings =
        reinterpret_cast<void*>(GetGlobalValue_6C5760());
    const float distanceScale = CSF_AT(float, distanceSettings, 0x130);
    const float distanceSquared =
        (dz * dz + dy * dy + dx * dx) * distanceScale * distanceScale;

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalObject, nullptr));
    void* provider = GetCachedObjectProvider(accessor, nullptr);
    float lodScale =
        static_cast<float>(GfxObject_GetLodDistanceScale(provider, nullptr));

    auto* flags =
        reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    if (static_cast<std::uint8_t>(
            C_008656d0_IsMaskSubset(flags, nullptr, 0x8000)) != 0)
    {
        lodScale *= g_HalfDurationScale;
    }

    const float nearDistance = lodScale * self->field_0070;
    const float farDistance = lodScale * self->field_0074;

    std::int32_t minimumDistanceBits;
    std::memcpy(
        &minimumDistanceBits, &self->field_0078, sizeof(minimumDistanceBits));
    if (minimumDistanceBits == -1082130432 ||
        distanceSquared < self->field_0078)
    {
        self->field_0078 = distanceSquared;
    }

    if (!(nearDistance > g_fZero))
        return 0;

    if (farDistance > g_fZero &&
        farDistance * farDistance < distanceSquared)
    {
        return 2;
    }

    return nearDistance * nearDistance < distanceSquared ? 1 : 0;
}

HOOK(0x006B30B0, S_006b30b0_ClassifyDistanceLod, "auto");
