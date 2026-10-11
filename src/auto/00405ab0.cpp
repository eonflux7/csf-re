#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_DispatchGlobalValueMessage, 0x00522330, (void* self, void* , const void* message));
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __cdecl Audio_PlayResourceAtPosition(
    void* position, const int* resourceId, char submitWithListener)
{
    alignas(4) std::uint8_t message[0x10]{};
    alignas(4) std::uint8_t request[0x38]{};

    const int initialX = CSF_AT(int, position, 0x04);
    const int initialY = CSF_AT(int, position, 0x08);
    CSF_AT(void*, message, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, message, 0x04) = initialX;
    CSF_AT(int, message, 0x08) = initialY;
    CSF_AT(int, message, 0x0C) = 1232348240;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    AudioSoundManager_DispatchGlobalValueMessage(manager, nullptr, message);

    const int currentY = CSF_AT(int, position, 0x08);
    CSF_AT(char, request, 0x35) = 1;
    CSF_AT(char, request, 0x36) = 1;
    CSF_AT(int, request, 0x08) = -1;
    CSF_AT(int, request, 0x0C) = -1;
    const int currentX = CSF_AT(int, position, 0x04);
    CSF_AT(int, request, 0x18) = 0;
    CSF_AT(int, request, 0x1C) = 0;
    CSF_AT(int, request, 0x20) = 0;
    CSF_AT(void*, request, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, request, 0x10) = 0;
    CSF_AT(int, request, 0x14) = 0;
    CSF_AT(char, request, 0x34) = 0;
    CSF_AT(int, request, 0x30) = 0;
    reinterpret_cast<S_00526d40*>(request)->field_0000 = 2;
    CSF_AT(int, request, 0x28) = currentX;
    CSF_AT(int, request, 0x2C) = currentY;
    CSF_AT(int, request, 0x04) = 3;

    auto* scene = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
    CSF_AT(int, request, 0x08) =
        SceneLoader_GetResourceById(scene + 0x22A4, nullptr, *resourceId);

    if (submitWithListener)
    {
        const int listenerX = CSF_AT(int, request, 0x28);
        const int listenerY = CSF_AT(int, request, 0x2C);
        CSF_AT(int, message, 0x04) = listenerX;
        CSF_AT(void*, message, 0x00) = CSF_VTBL(0x0082D448);
        CSF_AT(int, message, 0x08) = listenerY;
        CSF_AT(int, message, 0x0C) = 1232348240;

        auto* soundManager =
            reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
        return S_00521df0_SubmitSoundRequest(
            soundManager, nullptr, request, reinterpret_cast<int>(message));
    }

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_PlaySoundRequest(soundManager, nullptr, request, -1.0f);
}

HOOK(0x00405AB0, Audio_PlayResourceAtPosition, "auto");
