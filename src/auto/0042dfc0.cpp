#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the top and bottom letterbox bar heights of the HUD into the caller's
// two output slots. Returns the bottom value. Matches the original ret 8.
static int __fastcall CHud_GetLetterboxBounds(CHud* self, void* /*edx*/,
                                              uint32_t* outTop, uint32_t* outBottom) {
    *outTop = self->m_letterboxTop;
    uint32_t bottom = self->m_letterboxBottom;
    *outBottom = bottom;
    return static_cast<int>(bottom);
}

HOOK(0x0042DFC0, CHud_GetLetterboxBounds, "auto");
