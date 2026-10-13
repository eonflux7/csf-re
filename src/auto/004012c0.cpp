#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, MapEffectVariant, 0x00401100, (int a1));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0082d46c_ApplyScaledEffect(
    C_0082d46c* self, void* /*edx*/, int effectAddress)
{
    (void)self;
    auto* effect = reinterpret_cast<uint8_t*>(effectAddress);
    if (CSF_AT(int32_t, effect, 0x1C) < 0 ||
        CSF_AT(uint8_t, effect, 0x71) != 0)
        return;

    void* control = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(effect + 0x14)));

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* resourceTable = CSF_AT(void*, manager, 0x1FC0);
    void* resource = reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, resourceTable, 0)[0x8 / 4])(
            resourceTable, nullptr, CSF_AT(int, effect, 0x24));
    auto* resourceData = static_cast<uint8_t*>(resource) + 4;

    void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    const float scale = CSF_AT(float, settings, 0x140);
    const float scaledX = scale * CSF_AT(float, effect, 0x4C);
    const float scaledY = scale * CSF_AT(float, effect, 0x50);
    const float scaledZ = scale * CSF_AT(float, effect, 0x54);
    const float resourceScale =
        static_cast<float>(CSF_AT(int32_t, resourceData, 0x118));

    alignas(8) uint8_t vectorBytes[0x1C] = {};
    float* vector = reinterpret_cast<float*>(vectorBytes);
    vector[0] = scaledX * resourceScale;
    vector[1] = scaledY * resourceScale;
    vector[2] = scaledZ * resourceScale;

    void* owner = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x28 / 4])(control, nullptr);
    void* receiver = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0x78 / 4])(owner, nullptr);

    if (!receiver)
        return;

    if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, receiver, 0)[0x28 / 4])(receiver, nullptr))
        return;

    const int variant =
        MapEffectVariant(static_cast<int>(CSF_AT(int8_t, effect, 0x64)));

    if ((CSF_AT(uint32_t, control, 0x170) & 0x2000) != 0)
    {
        void* timing = CSF_AT(void*, control, 0x38);
        const double progress =
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, timing, 0)[0xC4 / 4])(timing, nullptr);
        if (!(progress < g_HalfDurationScale))
            return;
    }

    reinterpret_cast<void (__fastcall*)(void*, void*, float*, void*, int)>(
        CSF_AT(void**, receiver, 0)[0x44 / 4])(
            receiver, nullptr, vector, effect + 0x34, variant);
}

HOOK(0x004012C0, C_0082d46c_ApplyScaledEffect, "auto");
