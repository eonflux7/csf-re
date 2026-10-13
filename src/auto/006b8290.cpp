#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_Float_8A7E40, 0x008A7E40);

static double __fastcall C_0085ec18_GetAttachmentFirstWeight(C_0085ec18* self, void* /*edx*/, char a2) {
    void* attachment = CSF_AT(void*, self, 0x10);
    if (attachment == nullptr) {
        return g_fZero;
    }
    float weight = CSF_AT(float, attachment, 0x1C);
    if (a2 != 0) {
        return weight * g_Float_8A7E40;
    }
    return weight;
}

HOOK_KEEP_ECX(0x006B8290, C_0085ec18_GetAttachmentFirstWeight, 1, "gfx_object_accessors");
