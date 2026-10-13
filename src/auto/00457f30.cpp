#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_FadeTimerThreshold, 0x0082DC84);

CSF_FUNC(char, __fastcall, HudPortraits_ResetFlashState, 0x00457D90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Advances the portrait flash/fade timer. While the timer is at or below the
// threshold it forwards a scaled value to the rendering objects; otherwise it
// resets the flash state and clears the timer.
static char __fastcall HudPortraits_UpdateAnimationTimer(void* self, void* /*edx*/, float deltaTime)
{
    if (CSF_AT(float, self, 0x20) <= g_FadeTimerThreshold) {
        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
        void* cachedProvider = GetCachedObjectProvider(
            reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
        void* referencedObject = GetReferencedObject(cachedProvider, nullptr);

        // slot 4 (offset 16) of referencedObject's vtable
        void* renderer = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, referencedObject, 0)[4])(referencedObject, nullptr);

        float scaledValue = deltaTime * CSF_AT(float, self, 0x24) * g_MessageBoxAnimationScale;

        // slot 16 (offset 64) of renderer's vtable
        return static_cast<char>(((int (__fastcall*)(void*, void*, float))
            CSF_AT(void**, renderer, 0)[16])(renderer, nullptr, scaledValue));
    }

    CSF_AT(int, self, 0x1C) = 2;
    char result = HudPortraits_ResetFlashState(self, nullptr);
    CSF_AT(float, self, 0x20) = 0.0f;
    return result;
}

HOOK(0x00457F30, HudPortraits_UpdateAnimationTimer, "auto");
