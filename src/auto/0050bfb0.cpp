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

// Advances the fade timer by elapsed time. Progress = accumulated / duration.
// When the accumulated time reaches the duration, progress is clamped to 1.0,
// the accumulator is reset to 0 and the state is reset to 0.
static void __fastcall CMenuScreenFader_AdvanceFadeTimer(CMenuScreenFader* self, void* /*edx*/, float elapsedTime)
{
    float accumulated = elapsedTime + self->field_0030;
    self->field_0030 = accumulated;
    self->m_fadeProgress = accumulated / self->m_fadeDuration;

    if (accumulated >= self->m_fadeDuration)
    {
        self->m_fadeProgress = 1.0f;
        self->field_0030 = 0.0f;
        self->m_state = 0;
    }
}

HOOK(0x0050BFB0, CMenuScreenFader_AdvanceFadeTimer, "auto");
