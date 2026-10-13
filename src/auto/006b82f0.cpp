#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_Float_8A7E40, 0x008A7E40);

static double __fastcall C_0085ec18_GetFadeWidth(C_0085ec18* self, void* /*edx*/, char a2) {
    void* fadeSource = CSF_AT(void*, self, 0x10);
    if (fadeSource == nullptr) {
        return g_fZero;
    }
    float width = CSF_AT(float, fadeSource, 0x24);
    if (a2 != 0) {
        return width * g_Float_8A7E40;
    }
    return width;
}

HOOK_KEEP_ECX(0x006B82F0, C_0085ec18_GetFadeWidth, 1, "gfx_object_accessors");
