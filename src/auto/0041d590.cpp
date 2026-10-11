#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_0082e428_PlayVehicleSound(
    C_0082e428* self, void* /*edx*/)
{
    alignas(4) std::uint8_t request[0x38]{};

    CSF_AT(int, request, 0x00) = 2;
    CSF_AT(int, request, 0x04) = 3;
    CSF_AT(int, request, 0x08) = -1;
    CSF_AT(int, request, 0x0C) = -1;
    CSF_AT(void**, request, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(std::uint8_t, request, 0x34) = 0;
    CSF_AT(std::uint8_t, request, 0x35) = 1;
    CSF_AT(std::uint8_t, request, 0x36) = 1;

    auto* soundSource = reinterpret_cast<std::uint8_t*>(self->field_0004);
    CSF_AT(std::uint32_t, request, 0x28) =
        CSF_AT(std::uint32_t, soundSource, 0x20);
    CSF_AT(std::uint32_t, request, 0x2C) =
        CSF_AT(std::uint32_t, soundSource, 0x24);

    auto* sceneLoader =
        reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
    CSF_AT(int, request, 0x08) =
        SceneLoader_GetResourceById(sceneLoader, nullptr, 47);

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_PlaySoundRequest(soundManager, nullptr, request, -1.0f);
}

HOOK(0x0041D590, C_0082e428_PlayVehicleSound, "auto");
