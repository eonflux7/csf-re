#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __cdecl, GfxFrames_ComputeClumpLocalBoundingSphere, 0x006C0D70, (std::uint32_t* result, int clump));

CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

static char __fastcall C_0085e7b8_InitializeAttachment(
    C_0085e7b8* self, void* /*edx*/, C_0085e7b8* attachment)
{
    alignas(8) std::uint8_t sphereBytes[0x10];

    if (CSF_AT(std::uint8_t, self, 0x00))
        CSF_AT(std::uint8_t, self, 0x00) = 0;

    CSF_AT(std::uint32_t, sphereBytes, 0x04) = 1287568416;
    self->field_0004 = 1287568416;
    CSF_AT(std::uint32_t, sphereBytes, 0x08) = 1287568416;
    self->field_0008 = 1287568416;
    CSF_AT(std::uint32_t, sphereBytes, 0x0C) = 1287568416;
    self->field_000c = 1287568416;
    self->field_0010 = 1287568416;
    CSF_AT(std::uint32_t, self, 0x14) = 1287568416;
    CSF_AT(std::uint32_t, self, 0x18) = 1287568416;

    CSF_AT(float, self, 0x30) = 1.0f;
    CSF_AT(std::int32_t, self, 0x28) = 0;
    CSF_AT(std::int32_t, self, 0x2C) = 0;
    CSF_AT(std::int32_t, self, 0x20) = -1;
    CSF_AT(std::int32_t, self, 0x24) = 0;

    if (GetCachedObjectProvider(attachment, nullptr))
    {
        void* provider = GetCachedObjectProvider(attachment, nullptr);
        std::uint32_t* sphere = GfxFrames_ComputeClumpLocalBoundingSphere(
            reinterpret_cast<std::uint32_t*>(sphereBytes),
            static_cast<int>(reinterpret_cast<std::intptr_t>(provider)));
        CSF_AT(float, self, 0x1C) =
            CSF_AT(float, sphere, 0x0C) * g_ControlHoldThreshold;
    }

    CSF_AT(std::uint8_t, self, 0x00) = 1;
    return 1;
}

HOOK(0x006E4480, C_0085e7b8_InitializeAttachment, "auto");
