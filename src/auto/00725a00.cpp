#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __cdecl, ApplyCenteredRotation, 0x00725630,
         (int quad, float time));

static void __fastcall CGuiButton_ApplyPendingQuadTimes(
    CGuiButton* self, void* /*edx*/)
{
    std::uint8_t* timedFlag = &self->m_bIdleTimed;
    int remaining = 3;

    do {
        if (*timedFlag != 0) {
            const float time = CSF_AT(float, timedFlag, 4);
            const int quad = CSF_AT(int, timedFlag, -4);
            ApplyCenteredRotation(quad, time);
            *timedFlag = 0;
        }

        timedFlag += 0x1C;
        --remaining;
    } while (remaining != 0);

    if (self->m_bPermanentTimed != 0) {
        const float time = self->m_permanentTime;
        const int quad = static_cast<int>(self->m_pPermanentQuad);
        ApplyCenteredRotation(quad, time);
        self->m_bPermanentTimed = 0;
    }
}

HOOK(0x00725A00, CGuiButton_ApplyPendingQuadTimes, "auto");
