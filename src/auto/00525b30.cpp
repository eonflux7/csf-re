#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_40FE30, 0x0040FE30, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(int, __stdcall, sub_525490, 0x00525490, (int a1, int a2));
CSF_FUNC(double, __stdcall, sub_525630, 0x00525630, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, _rand, 0x007B04E7, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Plays a random voice/sound line for a character event, if the cooldowns allow it.
static void __fastcall C_00833908_SendCharacterEvent(C_00833908* self, void* /*edx*/, int character, int eventType)
{
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0)
        return;

    if (static_cast<uint32_t>(character) != GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    // A sound for this object is still active
    if (CSF_AT(int, self, 0x5C) != 0)
        return;

    // Global cooldown since the last event sound
    void* config = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(float, self, 0x60) < CSF_AT(float, config, 0x960))
        return;

    // Per-event-type cooldown
    double eventCooldown = sub_525630(eventType);
    if (eventCooldown > CSF_AT(float, self, eventType * 4 - 0x130))
        return;

    uint32_t roll = static_cast<uint32_t>(_rand() % 100);
    config = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (roll < CSF_AT(uint32_t, config, 0x98C))
    {
        int soundId = sub_525490(character, eventType);
        if (soundId != -1)
        {
            uint8_t request[0x38];
            std::memset(request, 0, sizeof(request));

            void* resourceTable = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
            int resource = SceneLoader_GetResourceById(resourceTable, nullptr, soundId);
            sub_40FE30(request, nullptr, 5, resource, 4, 0, -1);
            request[0x36] = 0;
            request[0x35] = 0;

            void* soundManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
            CSF_AT(int, self, 0x5C) = ApplyGlobalValueRequest(soundManager, nullptr, request, -1.0f);
        }
    }

    CSF_AT(float, self, eventType * 4 - 0x130) = 0.0f;
    CSF_AT(float, self, 0x60) = 0.0f;
}

HOOK(0x00525B30, C_00833908_SendCharacterEvent, "auto");
