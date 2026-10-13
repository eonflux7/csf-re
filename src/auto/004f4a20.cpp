#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004f2610_UpdateThumbnailObjects, 0x004F2930, (S_004f2610* self, void*));
CSF_FUNC(char, __fastcall, HudMenu_FinalizeThumbnailState, 0x004F4500, (void* self, void* edx));

// Updates the thumbnail objects at +4 and +8, then tail-jumps into the
// finalization routine with the object at +12 and returns its result.
static char __fastcall HudMenu_UpdateThumbnails(void* self, void* /*edx*/)
{
    S_004f2610_UpdateThumbnailObjects(reinterpret_cast<S_004f2610*>(CSF_AT(void*, self, 0x4)), nullptr);
    S_004f2610_UpdateThumbnailObjects(reinterpret_cast<S_004f2610*>(CSF_AT(void*, self, 0x8)), nullptr);
    return HudMenu_FinalizeThumbnailState(CSF_AT(void*, self, 0xC), nullptr);
}

HOOK(0x004F4A20, HudMenu_UpdateThumbnails, "auto");
