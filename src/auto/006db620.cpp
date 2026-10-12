#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AppMain_ApplyRenderSettings, 0x006DA080, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AppMain_ReleaseHighQualityResources, 0x006DB160, (void* self, void* edx));

// Sets the render quality level. Quality 3 is never replaced, and an unchanged level is ignored.
// Leaving quality 4 (when not flagged by field_0044) first releases the high-quality resources.
// Returns the low byte of the working quality value, as the original does.
static char __fastcall AppMain_SetRenderQuality(S_006d24a0* self, void* /*edx*/, int requestedQuality)
{
    uint32_t currentQuality = self->field_00c8;

    if (currentQuality != static_cast<uint32_t>(requestedQuality) && currentQuality != 3)
    {
        if (currentQuality == 4 && requestedQuality != 4 && !self->field_0044)
            AppMain_ReleaseHighQualityResources(self, nullptr);

        self->field_00c8 = static_cast<uint32_t>(requestedQuality);
        char applied = AppMain_ApplyRenderSettings(self, nullptr);
        currentQuality = (currentQuality & 0xFFFFFF00u) | static_cast<uint8_t>(applied);
    }

    return static_cast<char>(currentQuality);
}

HOOK(0x006DB620, AppMain_SetRenderQuality, "auto");
