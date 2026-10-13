#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(int, __fastcall, CCamera_Construct, 0x006D32F0, (CCamera* self, void*));
CSF_FUNC(int, __fastcall, C_008655e8_Setup, 0x006D3450, (C_008655e8* self, void*, int setupAddress, uint32_t context));
CSF_FUNC(char, __fastcall, C_00862b58_ReleaseScreenEffect, 0x006F8020, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00862b58_LoadNamedMaterial, 0x006F81A0, (void* self, void* edx, char* String2));
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_WaterCameraNearClip, 0x00860DA4);
CSF_GLOBAL(float, g_WaterCameraFarClip, 0x00860DA8);
CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);

// Initial data values, documented separately from the mutable runtime globals.
[[maybe_unused]] static constexpr std::uint32_t kWaterCameraNearClipDefaultBits = 0x41200000;
[[maybe_unused]] static constexpr std::uint32_t kWaterCameraFarClipDefaultBits = 0x461C4000;

static char __fastcall C_00862b58_InitializeWithCamera(
    C_00862b58* self, void* /*edx*/, int effectAddress, int context)
{
    if (self->field_0008)
    {
        if (self->field_0060)
        {
            void** resource = reinterpret_cast<void**>(self->field_0060);
            int globalManager = GetGlobalInt();
            int rwManager = ResolveRwManager(globalManager, nullptr);
            ReleaseManagedResource(reinterpret_cast<void*>(rwManager), nullptr, resource);
            self->field_0060 = 0;
        }

        void* oldCamera = CSF_AT(void*, self, 0x6C);
        if (oldCamera)
        {
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, oldCamera, 0)[1])(oldCamera, nullptr, 1);
            CSF_AT(std::uint32_t, self, 0x6C) = 0;
        }
    }

    ScreenEffects_UpdateBase(self, nullptr);
    InitializeScreenEffect(self, nullptr, effectAddress);

    self->field_0060 = 0;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint32_t, self, 0x68) = 0;

    int cameraStorage = CoreMemory_AllocUntagged(228);
    C_008655e8* camera = nullptr;
    if (cameraStorage)
    {
        camera = reinterpret_cast<C_008655e8*>(
            CCamera_Construct(reinterpret_cast<CCamera*>(cameraStorage), nullptr));
    }
    CSF_AT(C_008655e8*, self, 0x6C) = camera;

    alignas(8) std::uint8_t cameraSetupBytes[0x24];
    CSF_AT(float, cameraSetupBytes, 0x10) = g_WaterCameraNearClip;
    CSF_AT(float, cameraSetupBytes, 0x14) = g_WaterCameraFarClip;
    CSF_AT(std::uint32_t, cameraSetupBytes, 0x1C) = g_PreviewCameraAngleBits;
    std::memset(cameraSetupBytes, 0, 16);
    CSF_AT(std::uint32_t, cameraSetupBytes, 0x20) = 0;
    CSF_AT(std::uint32_t, cameraSetupBytes, 0x18) = 0;

    C_008655e8_Setup(camera, nullptr,
        reinterpret_cast<int>(cameraSetupBytes),
        static_cast<std::uint32_t>(context));

    CSF_AT(std::uint32_t, self, 0x70) = 0;
    CSF_AT(std::uint8_t, self, 0x74) = 0;
    self->vSlot2C();

    char result = C_00862b58_LoadNamedMaterial(
        self, nullptr, CSF_AT(char*, reinterpret_cast<void*>(effectAddress), 0x4));
    self->field_0008 = 1;
    if (!result)
        C_00862b58_ReleaseScreenEffect(self, nullptr);

    return result;
}

HOOK(0x006F8270, C_00862b58_InitializeWithCamera, "auto");
