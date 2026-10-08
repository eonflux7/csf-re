#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Constant 1.0f used as the fade-alpha base (g_TransitionOne)
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Advances the fade timer by deltaSeconds, updates the overlay alpha
// (1 - elapsed / duration), and when the duration is reached resets the
// progress/alpha and moves the fader to state 2 (switch screen).
static void __fastcall CMenuScreenFader_AdvanceFade(CMenuScreenFader* self, void* /*edx*/, float deltaSeconds)
{
    float elapsed = deltaSeconds + self->field_0030;
    self->field_0030 = elapsed;
    self->m_fadeProgress = g_TransitionOne - elapsed / self->field_0028;

    // Unordered (NaN) compares leave the reset branch untaken, matching the original jne.
    if (elapsed >= self->field_0028)
    {
        self->field_0030 = 0.0f;
        self->m_fadeProgress = 0.0f;
        self->m_state = 2;
    }
}

HOOK(0x0050BF70, CMenuScreenFader_AdvanceFade, "auto");
