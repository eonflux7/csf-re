#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

CSF_GLOBAL(float, g_WaterCameraNearClip, 0x00860DA4);
CSF_GLOBAL(float, g_WaterCameraFarClip, 0x00860DA8);
CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);

// Initial values of the referenced data, not substitutes for runtime loads.
static_assert(__builtin_bit_cast(std::uint32_t, 10.0f) == 0x41200000);
static_assert(__builtin_bit_cast(std::uint32_t, 10000.0f) == 0x461C4000);

static int __fastcall S_006d4450_FinalizeObjectProvider(
    S_006d4450* self, void* /*edx*/)
{
    const std::uint32_t begin = self->field_0008;
    const int count = begin
        ? static_cast<std::int32_t>(self->field_000c - begin) / 60
        : 0;

    std::uint32_t remaining = static_cast<std::uint32_t>(count);
    std::uint32_t offset = 0;
    while (remaining != 0)
    {
        const std::uint32_t entryAddress = self->field_0008 + offset;
        void* entry = reinterpret_cast<void*>(entryAddress);
        if (CSF_AT(std::uint32_t, entry, 0x1C) != 0)
        {
            auto* vtable = reinterpret_cast<void**>(self->field_0000);
            reinterpret_cast<void(__fastcall*)(
                S_006d4450*, void*, std::uint32_t)>(vtable[0x10 / 4])(
                    self, nullptr, entryAddress);
        }
        offset += 60;
        --remaining;
    }

    alignas(8) std::uint8_t cameraParameters[0x28]{};
    CSF_AT(float, cameraParameters, 0x10) = g_WaterCameraNearClip;
    CSF_AT(float, cameraParameters, 0x14) = g_WaterCameraFarClip;
    CSF_AT(std::uint32_t, cameraParameters, 0x1C) = g_PreviewCameraAngleBits;

    void* manager = reinterpret_cast<void*>(
        ResolveRwManager(GetGlobalInt(), nullptr));
    C_0085e7b8* provider =
        reinterpret_cast<C_0085e7b8*(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, manager, 0)[0x10 / 4])(
                manager, nullptr, cameraParameters);

    GetCachedObjectProvider(provider, nullptr);
    GetGlobalInt();
    manager = reinterpret_cast<void*>(
        ResolveRwManager(GetGlobalInt(), nullptr));

    return reinterpret_cast<int(__fastcall*)(void*, void*, C_0085e7b8*)>(
        CSF_AT(void**, manager, 0)[0x14 / 4])(
            manager, nullptr, provider);
}

HOOK(0x006D4450, S_006d4450_FinalizeObjectProvider, "auto");
