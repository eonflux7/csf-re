#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Color scale constant (255.0f) used both as the alpha multiplier and as the overlay alpha maximum.
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

// Fade state-machine steps, called with the frame delta as the single stack argument.
CSF_FUNC(void, __fastcall, CMenuScreenFader_AdvanceFade, 0x0050BF70,
         (CMenuScreenFader* self, void* edx, float deltaSeconds));
CSF_FUNC(void, __fastcall, CMenuScreenFader_AdvanceFadeTimer, 0x0050BFB0,
         (CMenuScreenFader* self, void* edx, float deltaSeconds));
CSF_FUNC(void*, __fastcall, CMenuScreenFader_SwitchScreenByName, 0x0050C1B0,
         (CMenuScreenFader* self, void* edx, float deltaSeconds));

// Advances the fader state machine by one frame, then pushes the black overlay alpha
// (255 - progress * 255) into the overlay object's vtable slot +0x2C.
static int __fastcall CMenuScreenFader_Update(CMenuScreenFader* self, void* /*edx*/, float deltaSeconds)
{
    // The state is read once and cached, as the original does (eax holds it).
    const uint32_t state = self->m_state;
    if (state != 0) {
        switch (state) {
        case 1:
            CMenuScreenFader_AdvanceFade(self, nullptr, deltaSeconds);
            break;
        case 2:
            CMenuScreenFader_SwitchScreenByName(self, nullptr, deltaSeconds);
            break;
        case 3:
            CMenuScreenFader_AdvanceFadeTimer(self, nullptr, deltaSeconds);
            break;
        default:
            break;
        }
    }

    // The overlay pointer and progress are re-read after the state step, as in the original.
    const uint32_t overlayAddress = self->m_pOverlay;
    if (overlayAddress == 0) {
        return 0;
    }

    // Arguments block {0, 0, 0, 255 - progress * 255}; the original computes this in
    // extended precision, so the double-based arithmetic here matches its rounding.
    const double scaledProgress = static_cast<double>(self->m_fadeProgress) * static_cast<double>(g_ColorChannelScale);
    float colorArgs[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    colorArgs[3] = static_cast<float>(static_cast<double>(g_ColorChannelScale) - scaledProgress);

    void* overlayObject = reinterpret_cast<void*>(overlayAddress);
    // Vtable slot 0x2C / 4 = 11; thiscall with one stack argument (callee pops it).
    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, overlayObject, 0)[11])(
        overlayObject, nullptr, colorArgs);
}

HOOK(0x0050C280, CMenuScreenFader_Update, "auto");
