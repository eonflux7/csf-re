#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_0082e428_PlayWheelSound(
    C_0082e428* self, void* /*edx*/, char alternate)
{
    alignas(4) uint8_t request[0x38]{};
    alignas(4) uint8_t listenerInfo[0x10]{};

    CSF_AT(int, request, 0x28) = -1;
    CSF_AT(int, request, 0x2C) = -1;
    CSF_AT(int, request, 0x08) = -1;
    CSF_AT(int, request, 0x0C) = -1;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    CSF_AT(int, request, 0x04) = 1;
    CSF_AT(int, request, 0x18) = 0;
    CSF_AT(int, request, 0x1C) = 0;
    CSF_AT(int, request, 0x20) = 0;
    CSF_AT(void**, request, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, request, 0x10) = 0;
    CSF_AT(int, request, 0x14) = 0;
    CSF_AT(uint8_t, request, 0x34) = 0;
    CSF_AT(uint8_t, request, 0x35) = 1;
    CSF_AT(uint8_t, request, 0x36) = 1;
    CSF_AT(int, request, 0x30) = 0;
    CSF_AT(int, request, 0x00) = 2;
    CSF_AT(uint32_t, request, 0x28) = CSF_AT(uint32_t, owner, 0x20);
    CSF_AT(uint32_t, request, 0x2C) = CSF_AT(uint32_t, owner, 0x24);
    CSF_AT(int, request, 0x04) = 3;

    auto* scene = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    int resource = SceneLoader_GetResourceById(
        scene + 0x22A4, nullptr, 46 - (alternate != 0));

    CSF_AT(uint32_t, listenerInfo, 0x04) =
        CSF_AT(uint32_t, request, 0x28);
    CSF_AT(int, request, 0x08) = resource;
    CSF_AT(void**, listenerInfo, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, listenerInfo, 0x08) =
        CSF_AT(uint32_t, request, 0x2C);
    CSF_AT(float, listenerInfo, 0x0C) = 1000005.0f;

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_SubmitSoundRequest(
        soundManager, nullptr, request,
        reinterpret_cast<int>(listenerInfo));
}

HOOK(0x0041D640, C_0082e428_PlayWheelSound, "auto");
