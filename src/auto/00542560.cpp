#include <cstdint>
#include "globals.h"
#include "hook.h"

// Engine helpers bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuCharSelect_FinalizePreviewSlot, 0x0073E920, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MenuCharSelect_AdvancePreviewTimer, 0x0073EAA0, (void* self, void* edx, float seconds));

// Per-frame preview step, passed as the float argument (bits 0x3C23D70A).
static constexpr float kPreviewStepSeconds = 0.0099999998f;

// Runs the camera-preview wait loop while the global state reports phase 7,
// then finalizes the preview slot using the global dword value.
static int __cdecl MenuCharSelect_TickPreviewLoop()
{
    if (GetGlobalState_6A6D70())
    {
        void* state = GetGlobalState_6A6D70();
        // Vtable slot 0x1C: current phase query.
        int phase = ((int(__fastcall*)(void*, void*))CSF_AT(void**, state, 0)[7])(state, nullptr);
        if (phase == 7)
        {
            state = GetGlobalState_6A6D70();
            // Vtable slot 0x64: enter preview state.
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, state, 0)[25])(state, nullptr);
            do
            {
                GetGlobalDword_73C020();
                MenuCharSelect_AdvancePreviewTimer(GetGlobalDword_73C020(), nullptr, kPreviewStepSeconds);
                state = GetGlobalState_6A6D70();
                // Vtable slot 0x18: advance preview by one step.
                ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, state, 0)[6])(state, nullptr, kPreviewStepSeconds);
                state = GetGlobalState_6A6D70();
                phase = ((int(__fastcall*)(void*, void*))CSF_AT(void**, state, 0)[7])(state, nullptr);
            } while (phase == 3);
        }
    }
    void* slot = GetGlobalDword_73C020();
    return MenuCharSelect_FinalizePreviewSlot(slot, nullptr);
}

HOOK(0x542560, MenuCharSelect_TickPreviewLoop, "auto");
