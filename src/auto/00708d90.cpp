#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original RwCameraEndUpdate (0x628E40): forwarder, one stack arg, caller cleans up (add esp, 4).
CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x628E40, (void* camera));

// Ends the RenderWare camera update, then copies the pending value (field_001c) into the
// camera at +0x64 and clears it.
static int __fastcall GfxScreenEffects_EndDrawing(C_00862a80* self, void* /*edx*/) {
    int result = RwCameraEndUpdate(reinterpret_cast<void*>(self->field_0010));

    void* camera = reinterpret_cast<void*>(self->field_0010);
    CSF_AT(uint32_t, camera, 0x64) = self->field_001c;
    self->field_001c = 0;
    return result;
}

HOOK(0x708D90, GfxScreenEffects_EndDrawing, "auto");
