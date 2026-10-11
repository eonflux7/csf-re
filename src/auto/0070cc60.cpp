#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Destroys the RenderWare texture held at offset +4 (if any), then clears both
// the texture slot (+4) and the dword at +0. Returns the destroy result, or 0
// when no texture was held.
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

static int __fastcall WaterTexture_ReleaseTexture(void* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x4);
    if (result) {
        result = RwTextureDestroy(CSF_AT(int, self, 0x4));
    }
    CSF_AT(int, self, 0x4) = 0;
    CSF_AT(int, self, 0x0) = 0;
    return result;
}

HOOK(0x0070CC60, WaterTexture_ReleaseTexture, "auto");
