#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall AiAlert_PrepareDistractedAlertTransition(char* self, void* /*edx*/)
{
    auto* sceneLoader = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    const int resourceId =
        SceneLoader_GetResourceById(sceneLoader + 0x22A4, nullptr, 3);
    if (resourceId == -1)
        return resourceId;

    alignas(8) uint8_t requestBytes[0x38]{};
    void* request = requestBytes;

    CSF_AT(int, request, 0x04) = 1;
    CSF_AT(uint8_t, request, 0x35) = 1;
    CSF_AT(uint8_t, request, 0x36) = 1;
    CSF_AT(int, request, 0x18) = 0;
    CSF_AT(int, request, 0x1C) = 0;
    CSF_AT(int, request, 0x20) = 0;
    CSF_AT(void**, request, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, request, 0x28) = -1;
    CSF_AT(int, request, 0x2C) = -1;
    CSF_AT(int, request, 0x14) = 0;
    CSF_AT(uint8_t, request, 0x34) = 0;
    CSF_AT(int, request, 0x0C) = -1;
    CSF_AT(int, request, 0x30) = 0;
    CSF_AT(int, request, 0x00) = 2;
    CSF_AT(int, request, 0x08) = resourceId;
    CSF_AT(float, request, 0x10) = -1.0f;

    const int linkedValue = GetLinkedObjectValue_4DB5C0(self + 8, nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* positionOwner = CSF_AT(void*, linkedObject, 0x2C);
    CSF_AT(int, request, 0x28) = CSF_AT(int, positionOwner, 0x20);
    CSF_AT(int, request, 0x2C) = CSF_AT(int, positionOwner, 0x24);
    CSF_AT(int, request, 0x04) = 3;

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_PlaySoundRequest(soundManager, nullptr, request, -1.0f);
}

HOOK(0x004B3240, AiAlert_PrepareDistractedAlertTransition, "auto");
