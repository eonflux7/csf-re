#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall CGuiControl_SetDebugRect(
    CGuiControl* self, void* /*edx*/, char enabled)
{
    self->m_bDebugRect = static_cast<uint8_t>(enabled);

    if (!enabled) {
        self->m_bDebugRect = 0;
        return enabled;
    }

    const uint32_t existingRect = self->m_pDebugRect;
    if (existingRect != 0) {
        self->m_bDebugRect = 0;
        return static_cast<char>(existingRect);
    }

    void* renderer = reinterpret_cast<void*>(self->m_pRenderer);
    if (!renderer) {
        self->m_bDebugRect = 0;
        return static_cast<char>(existingRect);
    }

    const uint32_t createdRect =
        ((uint32_t(__fastcall*)(void*, void*))
            CSF_AT(void**, renderer, 0)[0x48 / 4])(renderer, nullptr);

    self->m_pDebugRect = createdRect;
    return static_cast<char>(createdRect);
}

HOOK(0x00718550, CGuiControl_SetDebugRect, "auto");
